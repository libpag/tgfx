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

namespace tgfx {


TGFX_TEST(RecordTest, RecordLayer) {
  auto layer = Layer::Make();
  layer->setAlpha(0.5f);
  layer->setBlendMode(BlendMode::Multiply);
  layer->setPosition(Point{100.0f, 200.0f});
  layer->setMatrix(Matrix::MakeScale(1.5f, 1.5f)); // 设置 matrix
  layer->setRasterizationScale(2.0f); // 设置 rasterizationScale
  layer->setVisible(true);
  layer->setShouldRasterize(true);
  
  // 设置其他属性
  layer->setName("TestLayer"); // 设置 name
  layer->setAllowsEdgeAntialiasing(true); // 设置 allowsEdgeAntialiasing
  layer->setAllowsGroupOpacity(true); // 设置 allowsGroupOpacity
  
  // 设置 filters
  // auto blendFilter = std::make_shared<BlendFilter>(BlendMode::Overlay);
  // auto blurFilter = std::make_shared<BlurFilter>(5.0f);
  // layer->setFilters({blendFilter, blurFilter});
  
  // 设置 mask
  auto maskLayer = Layer::Make();
  maskLayer->setName("MaskLayer");
  layer->setMask(maskLayer);
  
  // 设置 scrollRect
  Rect scrollRect = Rect::MakeLTRB(50.0f, 50.0f, 150.0f, 150.0f);
  layer->setScrollRect(scrollRect);
  
  // ------- 命令序列化 --------
  auto jsonStr = Recorder::FlushCommands();
  std::cout << jsonStr << std::endl;
  int rootUuid = layer->_uuid;
  // ------- 回放 --------

  std::map<int, Recordable*> objMap;
  Recorder::Replay(jsonStr, objMap);
  auto replayLayer = objMap[rootUuid];
  // TODO 验证replayLayer中的属性是否正确
  Layer* castedReplayLayer = static_cast<Layer*>(replayLayer);
  ASSERT_NE(castedReplayLayer, nullptr);
  ASSERT_FLOAT_EQ(castedReplayLayer->alpha(), 0.5f);
  ASSERT_EQ(castedReplayLayer->blendMode(), BlendMode::Multiply);
  ASSERT_EQ(castedReplayLayer->position().x, 100.0f);
  ASSERT_EQ(castedReplayLayer->position().y, 200.0f);
  ASSERT_EQ(castedReplayLayer->matrix(), Matrix::MakeScale(1.5f, 1.5f)); // 验证 matrix
  ASSERT_FLOAT_EQ(castedReplayLayer->rasterizationScale(), 2.0f); // 验证 rasterizationScale
  ASSERT_TRUE(castedReplayLayer->visible());
  ASSERT_TRUE(castedReplayLayer->shouldRasterize());
  
  // 验证额外属性
  ASSERT_EQ(castedReplayLayer->name(), "TestLayer"); // 验证 name
  ASSERT_TRUE(castedReplayLayer->allowsEdgeAntialiasing()); // 验证 allowsEdgeAntialiasing
  ASSERT_TRUE(castedReplayLayer->allowsGroupOpacity()); // 验证 allowsGroupOpacity
  
  // 验证 filters
  // ASSERT_EQ(castedReplayLayer->filters().size(), 2);
  // ASSERT_EQ(castedReplayLayer->filters()[0]->type(), LayerFilterType::Blend);
  // ASSERT_EQ(castedReplayLayer->filters()[1]->type(), LayerFilterType::Blur);
  
  // 验证 mask
  ASSERT_NE(castedReplayLayer->mask(), nullptr);
  ASSERT_EQ(castedReplayLayer->mask()->name(), "MaskLayer");
  
  // 验证 scrollRect
  ASSERT_EQ(castedReplayLayer->scrollRect(), scrollRect);
  
  // 检查是否有遗漏的接口
  // 如果设置了其他属性，如 additional properties, 应进行验证
}
}  // namespace tgfx
