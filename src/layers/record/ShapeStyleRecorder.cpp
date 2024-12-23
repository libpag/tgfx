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

#define ENABLE_METHOD_LOGGING 1
#include "core/utils/Log.h"

#include "layers/command/ShapeStyleCmd.h"

namespace tgfx {

// ----------------- SolidColor -----------------

/**
 * 录制创建 SolidColor
 */
void ShapeStyleRecorder::MakeSolidColor(ShapeStyle* style, const Color& color) {
    LOG_METHOD("");
    Recorder::Record(std::make_unique<CmdMakeSolidColor>(style->_uuid, color));
}

/**
 * 录制设置颜色
 */
void ShapeStyleRecorder::setColor(ShapeStyle* style, const Color& color) {
    LOG_METHOD("");
    Recorder::Record(std::make_unique<CmdSetSolidColor>(style->_uuid, color));
}

// ----------------- Gradient -----------------

void ShapeStyleRecorder::MakeLinear(ShapeStyle* style, const Point& startPoint,
                                    const Point& endPoint) {
    LOG_METHOD("");
}
void ShapeStyleRecorder::MakeRadial(ShapeStyle* style, const Point& center, float radius) {
    LOG_METHOD("");
}
void ShapeStyleRecorder::MakeConic(ShapeStyle* style, const Point& center, float startAngle,
                                   float endAngle) {
    LOG_METHOD("");
}
void ShapeStyleRecorder::setColors(ShapeStyle* style, const std::vector<Color>& colors) {
    LOG_METHOD("");
}
void ShapeStyleRecorder::setPositions(ShapeStyle* style, const std::vector<float>& positions) {
    LOG_METHOD("");
}
void ShapeStyleRecorder::setEndPoint(ShapeStyle* style, const Point& endPoint) {
    LOG_METHOD("");
}
void ShapeStyleRecorder::setStartPoint(ShapeStyle* style, const Point& startPoint) {
    LOG_METHOD("");
}
void ShapeStyleRecorder::setCenter(ShapeStyle* style, const Point& center) {
    LOG_METHOD("");
}
void ShapeStyleRecorder::setRadius(ShapeStyle* style, float radius) {
    LOG_METHOD("");
}
void ShapeStyleRecorder::setStartAngle(ShapeStyle* style, float startAngle) {
    LOG_METHOD("");
}
void ShapeStyleRecorder::setEndAngle(ShapeStyle* style, float endAngle){
    LOG_METHOD("");
}

}  // namespace tgfx