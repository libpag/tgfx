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

enum class StrokeAlign;
enum class LineCap;
enum class LineJoin;
class Path;
class Shape;
class ShapeStyle;
class ShaperLayer;

class ShapeLayerRecorder {
 public:
 private:
  // 录制创建 ShapeLayer
  static void MakeShapeLayer(ShaperLayer* layer);

  // 录制设置路径
  static void setPath(ShaperLayer* layer, Path path);

  // 录制设置形状
  static void setShape(ShaperLayer* layer, std::shared_ptr<Shape> shape);

  // 录制设置填充样式
  static void setFillStyle(ShaperLayer* layer, std::shared_ptr<ShapeStyle> style);

  // 录制设置描边样式
  static void setStrokeStyle(ShaperLayer* layer, std::shared_ptr<ShapeStyle> style);

  // 录制设置线帽样式
  static void setLineCap(ShaperLayer* layer, LineCap cap);

  // 录制设置线连接样式
  static void setLineJoin(ShaperLayer* layer, LineJoin join);

  // 录制设置斜接限制
  static void setMiterLimit(ShaperLayer* layer, float limit);

  // 录制设置线宽
  static void setLineWidth(ShaperLayer* layer, float width);

  // 录制设置虚线模式
  static void setLineDashPattern(ShaperLayer* layer, const std::vector<float>& pattern);

  // 录制设置虚线相位
  static void setLineDashPhase(ShaperLayer* layer, float phase);

  // 录制设置描边起始点
  static void setStrokeStart(ShaperLayer* layer, float start);

  // 录制设置描边结束点
  static void setStrokeEnd(ShaperLayer* layer, float end);

  // 录制设置描边对齐方式
  static void setStrokeAlign(ShaperLayer* layer, StrokeAlign align);

  friend class ShaperLayer;
};
}  // namespace tgfx
