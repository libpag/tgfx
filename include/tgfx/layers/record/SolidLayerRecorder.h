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

#pragma once
#include <string>

namespace tgfx {

class SolidLayer;
struct Color;

class SolidLayerRecorder {
 public:
 private:
  
  // 录制创建 SolidLayer
  static void MakeSolidLayer(SolidLayer* layer);

  // 录制设置宽度
  static void setWidth(SolidLayer* layer, float width);

  // 录制设置高度
  static void setHeight(SolidLayer* layer, float height);

  // 录制设置X轴半径
  static void setRadiusX(SolidLayer* layer, float radiusX);

  // 录制设置Y轴半径
  static void setRadiusY(SolidLayer* layer, float radiusY);

  // 录制设置颜色
  static void setColor(SolidLayer* layer, const Color& color);

  friend class SolidLayer;
};
}  // namespace tgfx
