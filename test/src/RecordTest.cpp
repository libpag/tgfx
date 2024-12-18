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

#include <math.h>
#include <tgfx/layers/record/Recorder.h>
#include <vector>
#include "core/filters/BlurImageFilter.h"
#include "core/shapes/AppendShape.h"
#include "core/shapes/EffectShape.h"
#include "core/shapes/GlyphShape.h"
#include "core/shapes/MatrixShape.h"
#include "core/shapes/MergeShape.h"
#include "core/shapes/PathShape.h"
#include "core/shapes/StrokeShape.h"
#include "core/utils/Profiling.h"
#include "tgfx/core/PathEffect.h"
#include "tgfx/layers/DisplayList.h"
#include "tgfx/layers/Gradient.h"
#include "tgfx/layers/ImageLayer.h"
#include "tgfx/layers/Layer.h"
#include "tgfx/layers/ShapeLayer.h"
#include "tgfx/layers/SolidLayer.h"
#include "tgfx/layers/TextLayer.h"
#include "tgfx/layers/filters/BlendFilter.h"
#include "tgfx/layers/filters/BlurFilter.h"
#include "tgfx/layers/filters/ColorMatrixFilter.h"
#include "tgfx/layers/filters/DropShadowFilter.h"
#include "tgfx/layers/filters/InnerShadowFilter.h"
#include "utils/TestUtils.h"
#include "utils/common.h"
#include "tgfx/core/Path.h"
#include <gtest/gtest.h>

namespace tgfx {

TGFX_TEST(RecordTest, RecordLayer) {
  auto rootLayer = Layer::Make();
  rootLayer->setAlpha(0.5f);
  rootLayer->setBlendMode(BlendMode::Multiply);
  rootLayer->setPosition(Point{100.0f, 200.0f});
  // layer 设置position后，应该是下面的值
  ASSERT_EQ(rootLayer->position().x, 100.0f);
  ASSERT_EQ(rootLayer->position().y, 200.0f);
  rootLayer->setMatrix(Matrix::MakeScale(1.5f, 1.5f));  // 设置 matrix
  // 继续设置matrix后，position会被覆盖掉，所以是0了
  ASSERT_EQ(rootLayer->position().x, 0);
  ASSERT_EQ(rootLayer->position().y, 0);

  rootLayer->setRasterizationScale(2.0f);  // 设置 rasterizationScale
  rootLayer->setVisible(true);
  rootLayer->setShouldRasterize(true);

  // 设置其他属性
  rootLayer->setName("TestLayer");             // 设置 name
  rootLayer->setAllowsEdgeAntialiasing(true);  // 设置 allowsEdgeAntialiasing
  rootLayer->setAllowsGroupOpacity(true);      // 设置 allowsGroupOpacity

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
  ASSERT_TRUE(rootLayer->addChild(childLayer1));
  ASSERT_TRUE(rootLayer->addChildAt(childLayer2, 0));
  ASSERT_EQ(rootLayer->children().size(), static_cast<size_t>(2));
  ASSERT_EQ(rootLayer->children()[0]->name(), "ChildLayer2");
  ASSERT_EQ(rootLayer->children()[1]->name(), "ChildLayer1");

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
  ASSERT_TRUE(rootLayer->addChild(shapeLayer));

  // ------- 序列化 --------
  auto jsonStr = Recorder::FlushCommands();
  std::cout << jsonStr << std::endl;
  int rootUuid = rootLayer->_uuid;
  // ------- 回放 --------

  std::map<int, std::shared_ptr<Recordable>> objMap;
  Recorder::Replay(jsonStr, objMap);
  auto replayLayer = objMap[rootUuid];
  auto castedReplayLayer = std::static_pointer_cast<Layer>(replayLayer);
  ASSERT_NE(castedReplayLayer, nullptr);
  ASSERT_FLOAT_EQ(castedReplayLayer->alpha(), 0.5f);
  ASSERT_EQ(castedReplayLayer->blendMode(), BlendMode::Multiply);

  // 这里同时还原设置了position和matrix，所以还原后的position是0
  ASSERT_EQ(castedReplayLayer->position().x, 0.0f);
  ASSERT_EQ(castedReplayLayer->position().y, 0.0f);
  ASSERT_EQ(castedReplayLayer->matrix(), Matrix::MakeScale(1.5f, 1.5f));  // 验证 matrix
  ASSERT_FLOAT_EQ(castedReplayLayer->rasterizationScale(), 2.0f);         // 验证 rasterizationScale
  ASSERT_TRUE(castedReplayLayer->visible());
  ASSERT_TRUE(castedReplayLayer->shouldRasterize());

  // 验证额外属性
  ASSERT_EQ(castedReplayLayer->name(), "TestLayer");         // 验证 name
  ASSERT_TRUE(castedReplayLayer->allowsEdgeAntialiasing());  // 验证 allowsEdgeAntialiasing
  ASSERT_TRUE(castedReplayLayer->allowsGroupOpacity());      // 验证 allowsGroupOpacity

  // 验证 filters
  // ASSERT_EQ(castedReplayLayer->filters().size(), static_cast<size_t>(2));
  // ASSERT_EQ(castedReplayLayer->filters()[0]->type(), LayerFilterType::Blend);
  // ASSERT_EQ(castedReplayLayer->filters()[1]->type(), LayerFilterType::Blur);

  // 验证 mask
  ASSERT_NE(castedReplayLayer->mask(), nullptr);
  ASSERT_EQ(castedReplayLayer->mask()->name(), "MaskLayer");

  // 验证 scrollRect
  ASSERT_EQ(castedReplayLayer->scrollRect(), scrollRect);

  // ------- 验证子层 --------
  ASSERT_EQ(castedReplayLayer->children().size(), static_cast<size_t>(3));
  ASSERT_EQ(castedReplayLayer->children()[0]->name(), "ChildLayer2");
  ASSERT_EQ(castedReplayLayer->children()[1]->name(), "ChildLayer1");

  // ------- 验证子层的进一步属性 --------
  ASSERT_FLOAT_EQ(castedReplayLayer->children()[0]->alpha(), 0.6f);  // 验证 ChildLayer2 的 alpha
  ASSERT_EQ(castedReplayLayer->children()[0]->blendMode(),
            BlendMode::Overlay);                             // 验证 ChildLayer2 的 blendMode
  ASSERT_TRUE(castedReplayLayer->children()[0]->visible());  // 验证 ChildLayer2 的 visible
  ASSERT_EQ(castedReplayLayer->children()[0]->matrix(),
            Matrix::MakeRotate(45.0f));  // 验证 ChildLayer2 的 matrix

  ASSERT_FLOAT_EQ(castedReplayLayer->children()[1]->alpha(), 0.8f);  // 验证 ChildLayer1 的 alpha
  ASSERT_EQ(castedReplayLayer->children()[1]->blendMode(),
            BlendMode::Screen);                               // 验证 ChildLayer1 的 blendMode
  ASSERT_FALSE(castedReplayLayer->children()[1]->visible());  // 验证 ChildLayer1 的 visible
  ASSERT_EQ(castedReplayLayer->children()[1]->matrix(),
            Matrix::MakeTrans(10.0f, 20.0f));  // 验证 ChildLayer1 的 matrix

  // 如果子层设置了其他属性，可以继续添加相应的断言
  // 例如，如果设置了 position，可以验证 position 是否正确
  // ASSERT_EQ(castedReplayLayer->children()[0]->position().x, expected_x);
  // ASSERT_EQ(castedReplayLayer->children()[0]->position().y, expected_y);

  // 在回放后，验证 ShapeLayer 的还原
  auto replayShapeLayer = std::static_pointer_cast<ShapeLayer>(objMap[shapeLayer->_uuid]);
  ASSERT_NE(replayShapeLayer, nullptr);
  ASSERT_FLOAT_EQ(replayShapeLayer->alpha(), 0.75f);
  ASSERT_EQ(replayShapeLayer->blendMode(), BlendMode::Overlay);
  ASSERT_EQ(replayShapeLayer->position().x, 50.0f);
  ASSERT_EQ(replayShapeLayer->position().y, 100.0f);
  ASSERT_EQ(replayShapeLayer->matrix(), Matrix::MakeScale(2.0f, 2.0f));
  ASSERT_FLOAT_EQ(replayShapeLayer->rasterizationScale(), 1.5f);
  ASSERT_TRUE(replayShapeLayer->visible());
  ASSERT_FALSE(replayShapeLayer->shouldRasterize());
  ASSERT_EQ(replayShapeLayer->name(), "TestShapeLayer");
  ASSERT_FALSE(replayShapeLayer->allowsEdgeAntialiasing());
  ASSERT_TRUE(replayShapeLayer->allowsGroupOpacity());

  // 验证 ShapeLayer 的路径和填充样式
  ASSERT_EQ(replayShapeLayer->path(), shapePath);
  ASSERT_NE(replayShapeLayer->fillStyle(), nullptr);
  auto solidColor = std::static_pointer_cast<SolidColor>(replayShapeLayer->fillStyle());
  ASSERT_EQ(solidColor->color(), Color::FromRGBA(255, 0, 255, 0));

  // 验证 ShapeLayer 被正确添加到 rootLayer 的子层中
  ASSERT_EQ(castedReplayLayer->children().size(), static_cast<size_t>(3));
  ASSERT_EQ(castedReplayLayer->children()[2]->name(), "TestShapeLayer");

  // 检查是否有遗漏的接口
  // ���果设置了其他属性，如 additional properties, 应进行验证
}

// ...existing code...

// 添加 PathShape 的单元测试
TGFX_TEST(RecordTest, PathShapeJson) {
    // 创建 PathShape 并测试序列化
    Path path;
    path.addRect(Rect::MakeLTRB(0.0f, 0.0f, 100.0f, 100.0f));
    auto pathShape = std::make_shared<PathShape>(std::move(path));
    std::string pathJson = pathShape->toJson();
    auto parsedPathShape = Shape::FromJson(pathJson);
    ASSERT_NE(parsedPathShape, nullptr);
    ASSERT_EQ(parsedPathShape->type(), Shape::Type::Path);
    // 验证 Path 内容
    Path originalPath = pathShape->getPath();
    Path parsedPath = parsedPathShape->getPath();
    ASSERT_EQ(originalPath, parsedPath);
}

// 添加 StrokeShape 的单元测试
TGFX_TEST(RecordTest, StrokeShapeJson) {
    // 创建 StrokeShape 并测试序列化
    Path path2;
    auto baseShape = std::make_shared<PathShape>(path2);
    Stroke stroke;
    stroke.width = 5.0f;
    auto strokeShape = std::make_shared<StrokeShape>(baseShape, stroke);
    std::string strokeJson = strokeShape->toJson();
    auto parsedStrokeShape = Shape::FromJson(strokeJson);
    ASSERT_NE(parsedStrokeShape, nullptr);
    ASSERT_EQ(parsedStrokeShape->type(), Shape::Type::Stroke);
    // 验证 Stroke 属性
    auto parsedStroke = std::static_pointer_cast<StrokeShape>(parsedStrokeShape)->stroke;
    ASSERT_EQ(stroke.width, parsedStroke.width);
    // 验证 内部 Shape
    ASSERT_EQ(std::static_pointer_cast<StrokeShape>(parsedStrokeShape)->shape->type(), Shape::Type::Path);
}

// 添加 MergeShape 的单元测试
TGFX_TEST(RecordTest, MergeShapeJson) {
    // 创建 MergeShape 并测试序列化
    Path path3;
    auto baseShape = std::make_shared<PathShape>(path3);
    auto secondShape = std::make_shared<PathShape>(path3);
    auto mergeShape = std::make_shared<MergeShape>(baseShape, secondShape, PathOp::Union);
    std::string mergeJson = mergeShape->toJson();
    auto parsedMergeShape = Shape::FromJson(mergeJson);
    ASSERT_NE(parsedMergeShape, nullptr);
    ASSERT_EQ(parsedMergeShape->type(), Shape::Type::Merge);
    // 验证 Merge 属性
    auto parsedMerge = std::static_pointer_cast<MergeShape>(parsedMergeShape);
    ASSERT_EQ(parsedMerge->pathOp, PathOp::Union);
    ASSERT_EQ(parsedMerge->first->type(), Shape::Type::Path);
    ASSERT_EQ(parsedMerge->second->type(), Shape::Type::Path);
}

// 添加 MatrixShape 的单元测试
TGFX_TEST(RecordTest, MatrixShapeJson) {
    // 创建 MatrixShape 并测试序列化
    Path path2;
    auto baseShape = std::make_shared<PathShape>(path2);
    Matrix matrix = Matrix::MakeScale(2.0f, 2.0f);
    auto matrixShape = std::make_shared<MatrixShape>(baseShape, matrix);
    std::string matrixJson = matrixShape->toJson();
    auto parsedMatrixShape = Shape::FromJson(matrixJson);
    ASSERT_NE(parsedMatrixShape, nullptr);
    ASSERT_EQ(parsedMatrixShape->type(), Shape::Type::Matrix);
    // 验证 Matrix 属性
    auto parsedMatrix = std::static_pointer_cast<MatrixShape>(parsedMatrixShape);
    ASSERT_EQ(parsedMatrix->matrix, matrix);
    ASSERT_EQ(parsedMatrix->shape->type(), Shape::Type::Path);
}

// 添加 GlyphShape 的单元测试
TGFX_TEST(RecordTest, GlyphShapeJson) {
    // 创建 GlyphShape 并测试序列化
    GlyphRun glyphRun;
    auto glyphRunList = std::make_shared<GlyphRunList>(glyphRun);
    // 假设 GlyphRunList 有添加 glyph 的方法
    // glyphRunList->addGlyph(...);
    auto glyphShape = std::make_shared<GlyphShape>(glyphRunList);
    std::string glyphJson = glyphShape->toJson();
    auto parsedGlyphShape = Shape::FromJson(glyphJson);
    ASSERT_NE(parsedGlyphShape, nullptr);
    ASSERT_EQ(parsedGlyphShape->type(), Shape::Type::Glyph);
    // 验证 GlyphRunList
    auto parsedGlyph = std::static_pointer_cast<GlyphShape>(parsedGlyphShape)->glyphRunList;
    ASSERT_EQ(glyphRunList, parsedGlyph);
}

// 添加 EffectShape 的单元测试
TGFX_TEST(RecordTest, EffectShapeJson) {
    // 创建 EffectShape 并测试序列化
    Path path2;
    auto baseShape = std::make_shared<PathShape>(path2);
    std::shared_ptr<PathEffect> pathEffect = PathEffect::MakeCorner(0);
    auto effectShape = std::make_shared<EffectShape>(baseShape, pathEffect);
    std::string effectJson = effectShape->toJson();
    auto parsedEffectShape = Shape::FromJson(effectJson);
    ASSERT_NE(parsedEffectShape, nullptr);
    ASSERT_EQ(parsedEffectShape->type(), Shape::Type::Effect);
    // 验证 Effect 属性
    auto parsedEffect = std::static_pointer_cast<EffectShape>(parsedEffectShape);
    ASSERT_EQ(parsedEffect->effect, pathEffect);
    ASSERT_EQ(parsedEffect->shape->type(), Shape::Type::Path);
}

// 添加 AppendShape 的单元测试
TGFX_TEST(RecordTest, AppendShapeJson) {
    // 创建 AppendShape 并测试序列化
    Path path2;
    auto baseShape = std::make_shared<PathShape>(path2);
    auto secondShape = std::make_shared<PathShape>(path2);
    std::vector<std::shared_ptr<Shape>> shapes = {baseShape, secondShape};
    auto appendShape = std::make_shared<AppendShape>(std::move(shapes));
    std::string appendJson = appendShape->toJson();
    auto parsedAppendShape = Shape::FromJson(appendJson);
    ASSERT_NE(parsedAppendShape, nullptr);
    ASSERT_EQ(parsedAppendShape->type(), Shape::Type::Append);
    // 验证 Append 属性
    auto parsedAppend = std::static_pointer_cast<AppendShape>(parsedAppendShape);
    ASSERT_EQ(parsedAppend->shapes.size(), 2u);
    ASSERT_EQ(parsedAppend->shapes[0]->type(), Shape::Type::Path);
    ASSERT_EQ(parsedAppend->shapes[1]->type(), Shape::Type::Path);
}


}  // namespace tgfx
