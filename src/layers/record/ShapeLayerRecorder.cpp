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
#include "tgfx/layers/record/ShapeLayerRecorder.h"
#include <tgfx/core/Path.h>
#include <tgfx/layers/record/Recorder.h>
#include "ShapeLayerCmd.h"
namespace tgfx {
void ShapeLayerRecorder::MakeShapeLayer(ShapeLayer* layer) {
  Recorder::Record(std::make_unique<CmdMakeShapeLayer>(layer->_uuid));
}
void ShapeLayerRecorder::setPath(ShapeLayer* layer, Path path) {
  Recorder::Record(std::make_unique<CmdSetPath>(layer->_uuid, path));
}
void ShapeLayerRecorder::setShape(ShapeLayer* layer, std::shared_ptr<Shape> shape) {
    Recorder::Record(std::make_unique<CmdSetShape>(layer->_uuid, shape));
}
void ShapeLayerRecorder::setFillStyle(ShapeLayer* layer, std::shared_ptr<ShapeStyle> style) {
    Recorder::Record(std::make_unique<CmdSetFillStyle>(layer->_uuid, style->_uuid));
}
void ShapeLayerRecorder::setStrokeStyle(ShapeLayer* layer, std::shared_ptr<ShapeStyle> style) {
    Recorder::Record(std::make_unique<CmdSetStrokeStyle>(layer->_uuid, style->_uuid));
}
void ShapeLayerRecorder::setLineCap(ShapeLayer* layer, LineCap cap) {
    Recorder::Record(std::make_unique<CmdSetLineCap>(layer->_uuid, cap));
}
void ShapeLayerRecorder::setLineJoin(ShapeLayer* layer, LineJoin join) {
    Recorder::Record(std::make_unique<CmdSetLineJoin>(layer->_uuid, join));
}
void ShapeLayerRecorder::setMiterLimit(ShapeLayer* layer, float limit) {
    Recorder::Record(std::make_unique<CmdSetMiterLimit>(layer->_uuid, limit));
}
void ShapeLayerRecorder::setLineWidth(ShapeLayer* layer, float width) {
    Recorder::Record(std::make_unique<CmdSetLineWidth>(layer->_uuid, width));
}
void ShapeLayerRecorder::setLineDashPattern(ShapeLayer* layer, const std::vector<float>& pattern) {
    Recorder::Record(std::make_unique<CmdSetLineDashPattern>(layer->_uuid, pattern));
}
void ShapeLayerRecorder::setLineDashPhase(ShapeLayer* layer, float phase) {
    Recorder::Record(std::make_unique<CmdSetLineDashPhase>(layer->_uuid, phase));
}
void ShapeLayerRecorder::setStrokeStart(ShapeLayer* layer, float start) {
    Recorder::Record(std::make_unique<CmdSetStrokeStart>(layer->_uuid, start));
}
void ShapeLayerRecorder::setStrokeEnd(ShapeLayer* layer, float end) {
    Recorder::Record(std::make_unique<CmdSetStrokeEnd>(layer->_uuid, end));
}
void ShapeLayerRecorder::setStrokeAlign(ShapeLayer* layer, StrokeAlign align) {
    Recorder::Record(std::make_unique<CmdSetStrokeAlign>(layer->_uuid, align));
}
}  // namespace tgfx