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

#include <tgfx/layers/record/Recordable.h>
#include <nlohmann/json.hpp>

namespace tgfx {
enum class CommandType {
  // LayerRecorder
  SetDefaultAllowsEdgeAntialiasing,
  SetDefaultAllowsGroupOpacity,
  MakeLayer,
  setName,
  setAlpha,
  setBlendMode,
  setPosition,
  setMatrix,
  setVisible,
  setShouldRasterize,
  setRasterizationScale,
  setAllowsEdgeAntialiasing,
  setAllowsGroupOpacity,
  setFilters,
  setMask,
  setScrollRect,
  addChildAt,
  removeChildAt,
  removeChildren,
  removeFromParent,
  setChildIndex,
  replaceChild,
  // ShapeLayerRecorder
  MakeShapeLayer,
  setPath,
  setShape,
  setFillStyle,
  setStrokeStyle,
  setLineCap,
  setLineJoin,
  setMiterLimit,
  setLineWidth,
  setLineDashPattern,
  setLineDashPhase,
  setStrokeStart,
  setStrokeEnd,
  setStrokeAlign
};

struct Command {
  static std::unique_ptr<Command> MakeFrom(const nlohmann::json& json);
  explicit Command() {
  }
  virtual ~Command() = default;
  virtual CommandType getType() const = 0;
  virtual void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) = 0;
  virtual nlohmann::json toJson() const = 0;
};

}  // namespace tgfx
