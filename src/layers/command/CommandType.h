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

#define DEFINE_COMMAND_TYPE(name) static constexpr int name = __COUNTER__;

namespace LayerCommandType {

DEFINE_COMMAND_TYPE(SetDefaultAllowsEdgeAntialiasing);
DEFINE_COMMAND_TYPE(SetDefaultAllowsGroupOpacity);
DEFINE_COMMAND_TYPE(MakeLayer);
DEFINE_COMMAND_TYPE(setName);
DEFINE_COMMAND_TYPE(setAlpha);
DEFINE_COMMAND_TYPE(setBlendMode);
DEFINE_COMMAND_TYPE(setPosition);
DEFINE_COMMAND_TYPE(setMatrix);
DEFINE_COMMAND_TYPE(setVisible);
DEFINE_COMMAND_TYPE(setShouldRasterize);
DEFINE_COMMAND_TYPE(setRasterizationScale);
DEFINE_COMMAND_TYPE(setAllowsEdgeAntialiasing);
DEFINE_COMMAND_TYPE(setAllowsGroupOpacity);
DEFINE_COMMAND_TYPE(setFilters);
DEFINE_COMMAND_TYPE(setMask);
DEFINE_COMMAND_TYPE(setScrollRect);
DEFINE_COMMAND_TYPE(addChildAt);
DEFINE_COMMAND_TYPE(removeChildAt);
DEFINE_COMMAND_TYPE(removeChildren);
DEFINE_COMMAND_TYPE(removeFromParent);
DEFINE_COMMAND_TYPE(setChildIndex);
DEFINE_COMMAND_TYPE(replaceChild);

};  // namespace LayerCommandType

namespace ShapeLayerCommandType {
// ShapeLayerRecorder
DEFINE_COMMAND_TYPE(MakeShapeLayer);
DEFINE_COMMAND_TYPE(setPath);
DEFINE_COMMAND_TYPE(setShape);
DEFINE_COMMAND_TYPE(setFillStyle);
DEFINE_COMMAND_TYPE(setStrokeStyle);
DEFINE_COMMAND_TYPE(setLineCap);
DEFINE_COMMAND_TYPE(setLineJoin);
DEFINE_COMMAND_TYPE(setMiterLimit);
DEFINE_COMMAND_TYPE(setLineWidth);
DEFINE_COMMAND_TYPE(setLineDashPattern);
DEFINE_COMMAND_TYPE(setLineDashPhase);
DEFINE_COMMAND_TYPE(setStrokeStart);
DEFINE_COMMAND_TYPE(setStrokeEnd);
DEFINE_COMMAND_TYPE(setStrokeAlign);
}  // namespace ShapeLayerCommandType

namespace ShapeStyleCommandType {
DEFINE_COMMAND_TYPE(MakeSolidColor);
DEFINE_COMMAND_TYPE(setColor);
}  // namespace ShapeStyleCommandType

namespace SolidLayerCommandType {
DEFINE_COMMAND_TYPE(MakeSolidLayer);
DEFINE_COMMAND_TYPE(setWidth);
DEFINE_COMMAND_TYPE(setHeight);
DEFINE_COMMAND_TYPE(setRadiusX);
DEFINE_COMMAND_TYPE(setRadiusY);
DEFINE_COMMAND_TYPE(setColor);
}  // namespace SolidLayerCommandType

namespace TextLayerCommandType {
DEFINE_COMMAND_TYPE(MakeTextLayer);
}  // namespace TextLayerCommandType
