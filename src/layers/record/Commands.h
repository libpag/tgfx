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
#include <tgfx/core/BlendMode.h>
#include <tgfx/core/Matrix.h>
#include <tgfx/core/Point.h>
#include <tgfx/layers/record/Recordable.h>
#include <nlohmann/json.hpp>

namespace tgfx {
enum class CommandType {
  SetDefaultAllowsEdgeAntialiasing,
  SetDefaultAllowsGroupOpacity,
  MakeLayer,
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
  replaceChild
};

struct Command {
  explicit Command() {
  }
  virtual ~Command() = default;
  virtual CommandType getType() const = 0;
  virtual void execute(std::map<int, Recordable*> objMap) = 0;
};

struct CmdSetDefaultAllowsEdgeAntialiasing : Command {
  bool _value;

  explicit CmdSetDefaultAllowsEdgeAntialiasing(bool value) : _value(value) {
  }

  CommandType getType() const override {
    return CommandType::SetDefaultAllowsEdgeAntialiasing;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetDefaultAllowsGroupOpacity : Command {
  bool _value;

  explicit CmdSetDefaultAllowsGroupOpacity(bool value) : _value(value) {
  }

  CommandType getType() const override {
    return CommandType::SetDefaultAllowsGroupOpacity;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdMakeLayer : Command {
  int _id;

  explicit CmdMakeLayer(int id) : _id(id) {
  }

  CommandType getType() const override {
    return CommandType::MakeLayer;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetAlpha : Command {
  int _id;
  float _alpha;

  CmdSetAlpha(int id, float alpha) : _id(id), _alpha(alpha) {
  }

  CommandType getType() const override {
    return CommandType::setAlpha;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetBlendMode : Command {
  int _id;
  BlendMode _blendMode;

  CmdSetBlendMode(int id, BlendMode blendMode) : _id(id), _blendMode(blendMode) {
  }

  CommandType getType() const override {
    return CommandType::setBlendMode;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetPosition : Command {
  int _id;
  Point _position;

  CmdSetPosition(int id, const Point& position) : _id(id), _position(position) {
  }

  CommandType getType() const override {
    return CommandType::setPosition;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetMatrix : Command {
  int _id;
  Matrix _matrix;

  CmdSetMatrix(int id, const Matrix& matrix) : _id(id), _matrix(matrix) {
  }

  CommandType getType() const override {
    return CommandType::setMatrix;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetVisible : Command {
  int _id;
  bool _visible;

  CmdSetVisible(int id, bool visible) : _id(id), _visible(visible) {
  }

  CommandType getType() const override {
    return CommandType::setVisible;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetShouldRasterize : Command {
  int _id;
  bool _shouldRasterize;

  CmdSetShouldRasterize(int id, bool shouldRasterize) : _id(id), _shouldRasterize(shouldRasterize) {
  }

  CommandType getType() const override {
    return CommandType::setShouldRasterize;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetRasterizationScale : Command {
  int _id;
  float _scale;

  CmdSetRasterizationScale(int id, float scale) : _id(id), _scale(scale) {
  }

  CommandType getType() const override {
    return CommandType::setRasterizationScale;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetAllowsEdgeAntialiasing : Command {
  int _id;
  bool _allows;

  CmdSetAllowsEdgeAntialiasing(int id, bool allows) : _id(id), _allows(allows) {
  }

  CommandType getType() const override {
    return CommandType::setAllowsEdgeAntialiasing;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetAllowsGroupOpacity : Command {
  int _id;
  bool _allows;

  CmdSetAllowsGroupOpacity(int id, bool allows) : _id(id), _allows(allows) {
  }

  CommandType getType() const override {
    return CommandType::setAllowsGroupOpacity;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetFilters : Command {
  int _id;
  std::vector<int> _filter_ids;  // 将 LayerFilter 替换为 id

  CmdSetFilters(int id, const std::vector<int>& filter_ids) : _id(id), _filter_ids(filter_ids) {
  }

  CommandType getType() const override {
    return CommandType::setFilters;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetMask : Command {
  int _id;
  int _mask_id;  // 将 Layer 替换为 id

  CmdSetMask(int id, int mask_id) : _id(id), _mask_id(mask_id) {
  }

  CommandType getType() const override {
    return CommandType::setMask;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetScrollRect : Command {
  int _id;
  Rect _rect;

  CmdSetScrollRect(int id, const Rect& rect) : _id(id), _rect(rect) {
  }

  CommandType getType() const override {
    return CommandType::setScrollRect;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdAddChildAt : Command {
  int _id;
  int _child_id;  // 将 Layer 替换为 id
  int _index;

  CmdAddChildAt(int id, int child_id, int index) : _id(id), _child_id(child_id), _index(index) {
  }

  CommandType getType() const override {
    return CommandType::addChildAt;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdRemoveChildAt : Command {
  int _id;
  int _index;

  CmdRemoveChildAt(int id, int index) : _id(id), _index(index) {
  }

  CommandType getType() const override {
    return CommandType::removeChildAt;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdRemoveChildren : Command {
  int _id;
  int _beginIndex;
  int _endIndex;

  CmdRemoveChildren(int id, int beginIndex, int endIndex)
      : _id(id), _beginIndex(beginIndex), _endIndex(endIndex) {
  }

  CommandType getType() const override {
    return CommandType::removeChildren;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdRemoveFromParent : Command {
  int _id;

  CmdRemoveFromParent(int id) : _id(id) {
  }

  CommandType getType() const override {
    return CommandType::removeFromParent;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdSetChildIndex : Command {
  int _id;
  int _child_id;  // 将 Layer 替换为 id
  int _index;

  CmdSetChildIndex(int id, int child_id, int index) : _id(id), _child_id(child_id), _index(index) {
  }

  CommandType getType() const override {
    return CommandType::setChildIndex;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

struct CmdReplaceChild : Command {
  int _id;
  int _oldChild_id;  // 将 Layer 替换为 id
  int _newChild_id;  // 将 Layer 替换为 id

  CmdReplaceChild(int id, int oldChild_id, int newChild_id)
      : _id(id), _oldChild_id(oldChild_id), _newChild_id(newChild_id) {
  }

  CommandType getType() const override {
    return CommandType::replaceChild;
  }

  void execute(std::map<int, Recordable*> objMap) override;
};

}  // namespace tgfx
