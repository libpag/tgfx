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

#include <tgfx/core/Color.h>
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
  setStrokeAlign,
  // SolidLayerRecorder
  MakeSolidLayer,
  setWidth,
  setHeight,
  setRadiusX,
  setRadiusY,
  setColor,

};

struct Command {
  int _id;  // 新增 _id 属性

  Command(int id) : _id(id) {
  }  // 修改构造函数

  static std::unique_ptr<Command> MakeFrom(const nlohmann::json& json);
  virtual ~Command() = default;
  virtual CommandType getType() const = 0;
  bool merge(const Command& other);
  virtual bool doMerge(const Command& other);

  virtual void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) = 0;
  virtual nlohmann::json toJson() const = 0;

  // ------------------------ 静态工具方法 begin ------------------------
  /**
   * 将 Color 对象转换为 JSON。
   * @param color 要转换的 Color 对象。
   * @return 转换后的 JSON 对象。
   */
  static nlohmann::json ColorToJson(const Color& color);

  /**
   * 从 JSON 创建一个 Color 对象。
   * @param json 包含颜色信息的 JSON 对象。
   * @return 创建的 Color 对象。
   */
  static Color JsonToColor(const nlohmann::json& json);
  // ------------------------ 静态工具方法 end ------------------------
};

}  // namespace tgfx
