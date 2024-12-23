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
#include "ShapeStyleRecorder.h"
#include <tgfx/layers/ShapeStyle.h>
#include <tgfx/layers/record/Recorder.h>
#include "layers/command/ShapeStyleCmd.h"

namespace tgfx {

// ----------------- SolidColor -----------------

/**
 * 录制创建 SolidColor
 */
void ShapeStyleRecorder::MakeSolidColor(ShapeStyle* style, const Color& color) {
    Recorder::Record(std::make_unique<CmdMakeSolidColor>(style->_uuid, color));
}

/**
 * 录制设置颜色
 */
void ShapeStyleRecorder::setColor(ShapeStyle* style, const Color& color) {
    Recorder::Record(std::make_unique<CmdSetSolidColor>(style->_uuid, color));
}

// ----------------- Gradient -----------------

void ShapeStyleRecorder::MakeLinear(ShapeStyle* style, const Point& startPoint,
                                    const Point& endPoint) {
}
void ShapeStyleRecorder::MakeRadial(ShapeStyle* style, const Point& center, float radius) {
}
void ShapeStyleRecorder::MakeConic(ShapeStyle* style, const Point& center, float startAngle,
                                   float endAngle) {
}
void ShapeStyleRecorder::setColors(ShapeStyle* style, const std::vector<Color>& colors) {
}
void ShapeStyleRecorder::setPositions(ShapeStyle* style, const std::vector<float>& positions) {
}
void ShapeStyleRecorder::setEndPoint(ShapeStyle* style, const Point& endPoint) {
}
void ShapeStyleRecorder::setStartPoint(ShapeStyle* style, const Point& startPoint) {
}
void ShapeStyleRecorder::setCenter(ShapeStyle* style, const Point& center) {
}
void ShapeStyleRecorder::setRadius(ShapeStyle* style, float radius) {
}
void ShapeStyleRecorder::setStartAngle(ShapeStyle* style, float startAngle) {
}
void ShapeStyleRecorder::setEndAngle(ShapeStyle* style, float endAngle){
}

}  // namespace tgfx