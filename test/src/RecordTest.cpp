/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Tencent is pleased to support the open source community by making tgfx available.
//
//  Copyright (C) 2024 THL A29 Limited, a Tencent company. All rights reserved.
//
//  Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
//  in compliance with the License. You may obtain a copy of the License at
//
//      https://opensource.org/licenses/BSD-3-Clause
//
//  unless required by applicable law or agreed to in writing, software distributed under the
//  license is distributed on an "as is" basis, without warranties or conditions of any kind,
//  either express or implied. see the license for the specific language governing permissions
//  and limitations under the license.
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <math.h>
#include <tgfx/layers/DisplayList.h>
#include <tgfx/layers/ImageLayer.h>
#include <tgfx/layers/record/Recorder.h>
#include <nlohmann/json.hpp>
#include <vector>
#include "core/Records.h"
#include "core/utils/Profiling.h"
#include "tgfx/core/Path.h"
#include "tgfx/layers/Gradient.h"
#include "tgfx/layers/Layer.h"
#include "tgfx/layers/ShapeLayer.h"
#include "tgfx/layers/SolidLayer.h"
#include "utils/TestUtils.h"
#include "utils/common.h"

// Debug: 画出来看看
#define GENERATOR_BASELINE_IMAGES = 1;

namespace tgfx {

// 定义测试夹具
class RecordTestFixture : public ::testing::Test {
 protected:
  std::shared_ptr<GLDevice> device;
  std::shared_ptr<Surface> surface;

  void SetUp() override {
    device = GLDevice::Make();
    auto context = device->lockContext();
    surface = Surface::Make(context, 400, 400);
  }

  void TearDown() override {
    device->unlock();
  }

  void compare(const std::shared_ptr<DisplayList>& display, const std::string& baselineKey) {
    display->render(surface.get());
    std::cout << " baselineKey: " << baselineKey << std::endl;
    EXPECT_TRUE(Baseline::Compare(surface, baselineKey));
  }

  std::string dump(const std::shared_ptr<Layer>& layer, const std::string& key = "test_image") {
    const std::shared_ptr displayList = std::make_unique<DisplayList>();
    displayList->root()->addChild(layer);
    return dump(displayList, key);
  }

  std::string dump(const std::shared_ptr<DisplayList>& display,
                   const std::string& key = "test_image") {
    display->render(surface.get());
#ifdef GENERATOR_BASELINE_IMAGES
    // 保存图片查看
    Baseline::Compare(surface, key);
#endif

    return Baseline::GetSurfaceMD5(surface);
  }
};

TEST_F(RecordTestFixture, RecordFromJson) {
  std::string jsonPath = ProjectPath::Absolute("test/src/record.json");
  auto jsonData = ReadFile(jsonPath);
  EXPECT_NE(jsonData, nullptr);

  auto jsonStr = std::string(reinterpret_cast<const char*>(jsonData->data()), jsonData->size());
  auto jsonArray = nlohmann::json::parse(jsonStr);

  auto objMap = std::map<int, std::shared_ptr<Recordable>>();
  std::shared_ptr<Layer> layer = nullptr;

  const std::shared_ptr displayList = std::make_unique<DisplayList>();

  ContextScope scope;
  auto context = scope.getContext();
  auto surface = Surface::Make(context, 1000, 1000);

  for (const auto& json : jsonArray) {
    int rootId = json["rootId"];
    std::string commands = json["commands"];
    Recorder::Replay(commands, objMap);

    if (layer == nullptr) {
      if (auto it = objMap.find(rootId); it != objMap.end()) {
        layer = std::static_pointer_cast<tgfx::Layer>(it->second);
        displayList->root()->addChild(layer);
      }
    }
    EXPECT_NE(layer, nullptr);
  }
  compare(displayList, "RecordTest/RecordFromJson");
}

TEST_F(RecordTestFixture, RecordLayer) {
  auto rootLayer = Layer::Make();
  rootLayer->setAlpha(0.5f);
  rootLayer->setBlendMode(BlendMode::Multiply);
  rootLayer->setPosition(Point{100.0f, 200.0f});
  // layer 设置position后，应该是下面的值
  EXPECT_EQ(rootLayer->position().x, 100.0f);
  EXPECT_EQ(rootLayer->position().y, 200.0f);
  rootLayer->setMatrix(Matrix::MakeScale(1.5f, 1.5f));  // 设置 matrix
  // 继续设置matrix后，position会被覆盖掉，所以是0了
  EXPECT_EQ(rootLayer->position().x, 0);
  EXPECT_EQ(rootLayer->position().y, 0);

  rootLayer->setRasterizationScale(2.0f);  // 设置 rasterizationScale
  rootLayer->setVisible(true);
  rootLayer->setShouldRasterize(true);

  // 设置其他属性
  rootLayer->setName("xxx");                   // 设置 name
  rootLayer->setAllowsEdgeAntialiasing(true);  // 设置 allowsEdgeAntialiasing
  rootLayer->setAllowsGroupOpacity(true);      // 设置 allowsGroupOpacity

  // 应该是9条命令
  EXPECT_EQ(Recorder::commands_.size(), static_cast<size_t>(9));
  rootLayer->setName("TestLayer");  // 设置 name
  // 再次设置name，命令应该被合并，还是9条
  EXPECT_EQ(Recorder::commands_.size(), static_cast<size_t>(9));

  // 设置 filters
  // auto blendFilter = std::make_shared<BlendFilter>(BlendMode::Overlay);
  // auto blurFilter = std::make_shared<BlurFilter>(5.0f);
  // layer->setFilters({blendFilter, blurFilter});

  // 设置 mask
  auto maskLayer = Layer::Make();
  maskLayer->setName("MaskLayer");
  rootLayer->setMask(maskLayer);

  // 设置 scrollRect
  Rect scrollRect = Rect::MakeLTRB(50.0f, 50.0f, 150.0f, 150.0f);
  rootLayer->setScrollRect(scrollRect);

  // ------- 子层管理 --------
  auto childLayer1 = Layer::Make();
  childLayer1->setName("ChildLayer1");
  // 设置 ChildLayer1 的属性
  childLayer1->setAlpha(0.8f);
  childLayer1->setBlendMode(BlendMode::Screen);
  childLayer1->setVisible(false);
  childLayer1->setMatrix(Matrix::MakeTrans(10.0f, 20.0f));

  auto childLayer2 = Layer::Make();
  childLayer2->setName("ChildLayer2");
  // 设置 ChildLayer2 的属性
  childLayer2->setAlpha(0.6f);
  childLayer2->setBlendMode(BlendMode::Overlay);
  childLayer2->setVisible(true);
  childLayer2->setMatrix(Matrix::MakeRotate(45.0f));

  // 添加子层
  EXPECT_TRUE(rootLayer->addChild(childLayer1));
  EXPECT_TRUE(rootLayer->addChildAt(childLayer2, 0));
  EXPECT_EQ(rootLayer->children().size(), static_cast<size_t>(2));
  EXPECT_EQ(rootLayer->children()[0]->name(), "ChildLayer2");
  EXPECT_EQ(rootLayer->children()[1]->name(), "ChildLayer1");

  // 构建 ShapeLayer 并设置属性
  auto shapeLayer = ShapeLayer::Make();
  shapeLayer->setAlpha(0.75f);
  shapeLayer->setBlendMode(BlendMode::Overlay);
  shapeLayer->setPosition(Point{50.0f, 100.0f});
  shapeLayer->setMatrix(Matrix::MakeScale(2.0f, 2.0f));
  shapeLayer->setRasterizationScale(1.5f);
  shapeLayer->setVisible(true);
  shapeLayer->setShouldRasterize(false);
  shapeLayer->setName("TestShapeLayer");
  shapeLayer->setAllowsEdgeAntialiasing(false);
  shapeLayer->setAllowsGroupOpacity(true);

  // 设置 ShapeLayer 的路径和样式
  Path shapePath;
  shapePath.addRect(Rect::MakeLTRB(0.0f, 0.0f, 100.0f, 100.0f));
  shapeLayer->setPath(shapePath);
  auto fillStyle = SolidColor::Make();
  fillStyle->setColor(Color::FromRGBA(255, 0, 255, 0));
  shapeLayer->setFillStyle(fillStyle);

  // 将 ShapeLayer 添加为 rootLayer 的子层
  EXPECT_TRUE(rootLayer->addChild(shapeLayer));

  // ------- 序列化 --------
  auto jsonStr = Recorder::FlushCommands();
  std::cout << jsonStr << std::endl;
  int rootUuid = rootLayer->_uuid;
  // ------- 回放 --------

  std::map<int, std::shared_ptr<Recordable>> objMap;
  Recorder::Replay(jsonStr, objMap);
  auto replayLayer = objMap[rootUuid];
  auto castedReplayLayer = std::static_pointer_cast<Layer>(replayLayer);
  EXPECT_NE(castedReplayLayer, nullptr);
  EXPECT_FLOAT_EQ(castedReplayLayer->alpha(), 0.5f);
  EXPECT_EQ(castedReplayLayer->blendMode(), BlendMode::Multiply);

  // 这里同时还原设置了position和matrix，所以还原后的position是0
  EXPECT_EQ(castedReplayLayer->position().x, 0.0f);
  EXPECT_EQ(castedReplayLayer->position().y, 0.0f);
  EXPECT_EQ(castedReplayLayer->matrix(), Matrix::MakeScale(1.5f, 1.5f));  // 验证 matrix
  EXPECT_FLOAT_EQ(castedReplayLayer->rasterizationScale(), 2.0f);         // 验证 rasterizationScale
  EXPECT_TRUE(castedReplayLayer->visible());
  EXPECT_TRUE(castedReplayLayer->shouldRasterize());

  // 验证额外属性
  EXPECT_EQ(castedReplayLayer->name(), "TestLayer");         // 验证 name
  EXPECT_TRUE(castedReplayLayer->allowsEdgeAntialiasing());  // 验证 allowsEdgeAntialiasing
  EXPECT_TRUE(castedReplayLayer->allowsGroupOpacity());      // 验证 allowsGroupOpacity

  // 验证 filters
  // EXPECT_EQ(castedReplayLayer->filters().size(), static_cast<size_t>(2));
  // EXPECT_EQ(castedReplayLayer->filters()[0]->type(), LayerFilterType::Blend);
  // EXPECT_EQ(castedReplayLayer->filters()[1]->type(), LayerFilterType::Blur);

  // 验证 mask
  EXPECT_NE(castedReplayLayer->mask(), nullptr);
  EXPECT_EQ(castedReplayLayer->mask()->name(), "MaskLayer");

  // 验证 scrollRect
  EXPECT_EQ(castedReplayLayer->scrollRect(), scrollRect);

  // ------- 验证子层 --------
  EXPECT_EQ(castedReplayLayer->children().size(), static_cast<size_t>(3));
  EXPECT_EQ(castedReplayLayer->children()[0]->name(), "ChildLayer2");
  EXPECT_EQ(castedReplayLayer->children()[1]->name(), "ChildLayer1");

  // ------- 验证子层的进一步属性 --------
  EXPECT_FLOAT_EQ(castedReplayLayer->children()[0]->alpha(), 0.6f);  // 验证 ChildLayer2 的 alpha
  EXPECT_EQ(castedReplayLayer->children()[0]->blendMode(),
            BlendMode::Overlay);                             // 验证 ChildLayer2 的 blendMode
  EXPECT_TRUE(castedReplayLayer->children()[0]->visible());  // 验证 ChildLayer2 的 visible
  EXPECT_EQ(castedReplayLayer->children()[0]->matrix(),
            Matrix::MakeRotate(45.0f));  // 验证 ChildLayer2 的 matrix

  EXPECT_FLOAT_EQ(castedReplayLayer->children()[1]->alpha(), 0.8f);  // 验证 ChildLayer1 的 alpha
  EXPECT_EQ(castedReplayLayer->children()[1]->blendMode(),
            BlendMode::Screen);                               // 验证 ChildLayer1 的 blendMode
  EXPECT_FALSE(castedReplayLayer->children()[1]->visible());  // 验证 ChildLayer1 的 visible
  EXPECT_EQ(castedReplayLayer->children()[1]->matrix(),
            Matrix::MakeTrans(10.0f, 20.0f));  // 验证 ChildLayer1 的 matrix

  // 如果子层设置了其他属性，可以继续添加相应的断言
  // 例如，如果设置了 position，可以验证 position 是否正确
  // EXPECT_EQ(castedReplayLayer->children()[0]->position().x, expected_x);
  // EXPECT_EQ(castedReplayLayer->children()[0]->position().y, expected_y);

  // 在回放后，验证 ShapeLayer 的还原
  auto replayShapeLayer = std::static_pointer_cast<ShapeLayer>(objMap[shapeLayer->_uuid]);
  EXPECT_NE(replayShapeLayer, nullptr);
  EXPECT_FLOAT_EQ(replayShapeLayer->alpha(), 0.75f);
  EXPECT_EQ(replayShapeLayer->blendMode(), BlendMode::Overlay);
  // 这里同时还原设置了position和matrix，所以还原后的position是0
  EXPECT_EQ(replayShapeLayer->position().x, 0.0f);
  EXPECT_EQ(replayShapeLayer->position().y, 0.0f);
  EXPECT_EQ(replayShapeLayer->matrix(), Matrix::MakeScale(2.0f, 2.0f));
  EXPECT_FLOAT_EQ(replayShapeLayer->rasterizationScale(), 1.5f);
  EXPECT_TRUE(replayShapeLayer->visible());
  EXPECT_FALSE(replayShapeLayer->shouldRasterize());
  EXPECT_EQ(replayShapeLayer->name(), "TestShapeLayer");
  EXPECT_FALSE(replayShapeLayer->allowsEdgeAntialiasing());
  EXPECT_TRUE(replayShapeLayer->allowsGroupOpacity());

  // 验证 ShapeLayer 的路径和填充样式
  // EXPECT_EQ(replayShapeLayer->path(), shapePath);
  EXPECT_NE(replayShapeLayer->fillStyle(), nullptr);
  //   auto solidColor = std::static_pointer_cast<SolidColor>(replayShapeLayer->fillStyle());
  //   EXPECT_EQ(solidColor->color().red, 255);
  // EXPECT_EQ(solidColor->color().green, 0);
  // EXPECT_EQ(solidColor->color().blue, 255);
  // EXPECT_EQ(solidColor->color().alpha, 0);
  // 验证 ShapeLayer 被正确添加到 rootLayer 的子层中
  EXPECT_EQ(castedReplayLayer->children().size(), static_cast<size_t>(3));
  EXPECT_EQ(castedReplayLayer->children()[2]->name(), "TestShapeLayer");

  // 检查是否有遗漏的接口
  // ���果设置了其他属性，如 additional properties, 应进行验证
}

TEST_F(RecordTestFixture, RecordShapeLayer) {

  auto shapeLayer = ShapeLayer::Make();
  shapeLayer->setAlpha(0.75f);
  shapeLayer->setBlendMode(BlendMode::Overlay);
  shapeLayer->setPosition(Point{50.0f, 100.0f});
  shapeLayer->setMatrix(Matrix::MakeScale(2.0f, 2.0f));
  shapeLayer->setRasterizationScale(1.5f);
  shapeLayer->setVisible(true);
  shapeLayer->setName("TestShapeLayer");

  shapeLayer->setLineWidth(10.0f);
  shapeLayer->setLineCap(LineCap::Butt);
  shapeLayer->setLineJoin(LineJoin::Miter);

  Path shapePath;
  shapePath.addRect(Rect::MakeLTRB(0.0f, 0.0f, 100.0f, 200.0f));
  shapeLayer->setPath(shapePath);

  auto fillStyle = SolidColor::Make();
  fillStyle->setColor(Color::FromRGBA(255, 0, 255, 0));
  shapeLayer->setFillStyle(fillStyle);

  auto strokeStyle = SolidColor::Make(Color::Red());
  shapeLayer->setStrokeStyle(strokeStyle);

  EXPECT_EQ(Recorder::commands_.size(), static_cast<size_t>(15));

  auto md5 = dump(shapeLayer, "RecordTest/RecordShapeLayer1");

  auto jsonStr = Recorder::FlushCommands();
  std::cout << jsonStr << std::endl;
  int shapeUuid = shapeLayer->_uuid;

  std::map<int, std::shared_ptr<Recordable>> objMap;
  Recorder::Replay(jsonStr, objMap);
  auto replayLayer = objMap[shapeUuid];
  auto castedReplayLayer = std::static_pointer_cast<ShapeLayer>(replayLayer);
  EXPECT_NE(castedReplayLayer, nullptr);
  EXPECT_FLOAT_EQ(castedReplayLayer->alpha(), 0.75f);
  EXPECT_EQ(castedReplayLayer->blendMode(), BlendMode::Overlay);

  EXPECT_EQ(castedReplayLayer->lineWidth(), 10.0f);
  EXPECT_EQ(castedReplayLayer->lineCap(), LineCap::Butt);
  EXPECT_EQ(castedReplayLayer->lineJoin(), LineJoin::Miter);

  EXPECT_NE(castedReplayLayer->fillStyle(), nullptr);
  auto solidColor = std::static_pointer_cast<SolidColor>(castedReplayLayer->fillStyle());
  EXPECT_EQ(solidColor->color(), Color::FromRGBA(255, 0, 255, 0));

  auto newMd5 = dump(castedReplayLayer, "RecordTest/RecordShapeLayer2");

  EXPECT_EQ(md5, newMd5);
  // ...更多属性断言...
}

TEST_F(RecordTestFixture, RecordSolidLayer) {
  auto solidLayer = SolidLayer::Make();
  solidLayer->setAlpha(0.6f);
  solidLayer->setBlendMode(BlendMode::Screen);
  solidLayer->setPosition(Point{150.0f, 250.0f});
  solidLayer->setMatrix(Matrix::MakeRotate(30.0f));
  solidLayer->setRasterizationScale(2.0f);
  solidLayer->setVisible(true);
  solidLayer->setShouldRasterize(true);
  solidLayer->setName("TestSolidLayer");
  solidLayer->setAllowsEdgeAntialiasing(true);
  solidLayer->setAllowsGroupOpacity(false);

  solidLayer->setWidth(100.0f);
  solidLayer->setHeight(200.0f);
  solidLayer->setRadiusX(10.0f);
  solidLayer->setRadiusY(20.0f);
  solidLayer->setColor(Color::FromRGBA(128, 128, 128, 255));

  EXPECT_EQ(Recorder::commands_.size(), static_cast<size_t>(13));

  auto jsonStr = Recorder::FlushCommands();
  std::cout << jsonStr << std::endl;
  int solidUuid = solidLayer->_uuid;

  std::map<int, std::shared_ptr<Recordable>> objMap;
  Recorder::Replay(jsonStr, objMap);
  auto replayLayer = objMap[solidUuid];
  auto castedReplayLayer = std::static_pointer_cast<SolidLayer>(replayLayer);
  EXPECT_NE(castedReplayLayer, nullptr);
  EXPECT_FLOAT_EQ(castedReplayLayer->alpha(), 0.6f);
  EXPECT_EQ(castedReplayLayer->blendMode(), BlendMode::Screen);
  // ...更多属性断言...
  EXPECT_FLOAT_EQ(castedReplayLayer->width(), 100.0f);
  EXPECT_FLOAT_EQ(castedReplayLayer->height(), 200.0f);
  EXPECT_FLOAT_EQ(castedReplayLayer->radiusX(), 10.0f);
  EXPECT_FLOAT_EQ(castedReplayLayer->radiusY(), 20.0f);
  EXPECT_EQ(castedReplayLayer->color(), Color::FromRGBA(128, 128, 128, 255));

  // ...更多属性断言...
}

TEST_F(RecordTestFixture, RecordImageLayer) {
  auto imageLayer = ImageLayer::Make();
  imageLayer->setAlpha(0.9f);
  imageLayer->setBlendMode(BlendMode::Multiply);
  imageLayer->setPosition(Point{300.0f, 400.0f});
  imageLayer->setMatrix(Matrix::MakeTrans(50.0f, 50.0f));
  imageLayer->setRasterizationScale(1.0f);
  imageLayer->setVisible(true);
  imageLayer->setShouldRasterize(false);
  imageLayer->setName("TestImageLayer");
  imageLayer->setAllowsEdgeAntialiasing(true);
  imageLayer->setAllowsGroupOpacity(true);

  imageLayer->setSampling(SamplingOptions(FilterMode::Nearest, MipmapMode::None));
  auto image = MakeImage("resources/apitest/rotation.jpg");
  imageLayer->setImage(image);

  EXPECT_EQ(Recorder::commands_.size(), static_cast<size_t>(10));

  auto jsonStr = Recorder::FlushCommands();
  std::cout << jsonStr << std::endl;
  int imageUuid = imageLayer->_uuid;

  std::map<int, std::shared_ptr<Recordable>> objMap;
  Recorder::Replay(jsonStr, objMap);
  auto replayLayer = objMap[imageUuid];
  auto castedReplayLayer = std::static_pointer_cast<ImageLayer>(replayLayer);
  EXPECT_NE(castedReplayLayer, nullptr);
  EXPECT_FLOAT_EQ(castedReplayLayer->alpha(), 0.9f);
  EXPECT_EQ(castedReplayLayer->blendMode(), BlendMode::Multiply);
  // ...更多属性断言...
  EXPECT_EQ(castedReplayLayer->sampling(), SamplingOptions(FilterMode::Nearest, MipmapMode::None));
  EXPECT_NE(castedReplayLayer->image(), nullptr);
  // 如果 Image 有更多属性，可以继续添加断言
  // 例如：
  // EXPECT_EQ(castedReplayLayer->image()->width(), expectedWidth);
  // EXPECT_EQ(castedReplayLayer->image()->height(), expectedHeight);

  // ...更多属性断言...
}

}  // namespace tgfx
