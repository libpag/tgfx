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
#include "layers/record/SolidLayerRecorder.h"
#include <tgfx/core/Path.h>
#include <tgfx/layers/SolidLayer.h>
#include <tgfx/layers/record/Recorder.h>
#include "SolidLayerCmd.h"
namespace tgfx {
void SolidLayerRecorder::MakeSolidLayer(SolidLayer* layer) {
    Recorder::Record(std::make_unique<CmdMakeSolidLayer>(layer->_uuid));
}
void SolidLayerRecorder::setWidth(SolidLayer* layer, float width) {
    Recorder::Record(std::make_unique<CmdSetWidth>(layer->_uuid, width));
}
void SolidLayerRecorder::setHeight(SolidLayer* layer, float height) {
    Recorder::Record(std::make_unique<CmdSetHeight>(layer->_uuid, height));
}
void SolidLayerRecorder::setRadiusX(SolidLayer* layer, float radiusX) {
    Recorder::Record(std::make_unique<CmdSetRadiusX>(layer->_uuid, radiusX));
}
void SolidLayerRecorder::setRadiusY(SolidLayer* layer, float radiusY) {
    Recorder::Record(std::make_unique<CmdSetRadiusY>(layer->_uuid, radiusY));
}
void SolidLayerRecorder::setColor(SolidLayer* layer, const Color& color) {
    Recorder::Record(std::make_unique<CmdSetColor>(layer->_uuid, color));
}
}  // namespace tgfx