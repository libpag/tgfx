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
#include <tgfx/core/Path.h>
#include <tgfx/core/Point.h>
#include <tgfx/core/Shape.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/ShapeStyle.h>
#include <tgfx/layers/record/Recordable.h>
#include <nlohmann/json.hpp>
#include "Commands.h"
#include "CommandType.h"

namespace tgfx {

class LayerCmdFactory {
 public:
  static std::unique_ptr<Command> MakeFrom(const nlohmann::json& json);
};

struct CmdSetDefaultAllowsEdgeAntialiasing : Command {
  bool _value;

  explicit CmdSetDefaultAllowsEdgeAntialiasing(int id, bool value) : Command(id), _value(value) {
  }

  int getType() const override {
    return LayerCommandType::SetDefaultAllowsEdgeAntialiasing;
  }
  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetDefaultAllowsGroupOpacity : Command {
  bool _value;

  explicit CmdSetDefaultAllowsGroupOpacity(int id, bool value) : Command(id), _value(value) {
  }

  int getType() const override {
    return LayerCommandType::SetDefaultAllowsGroupOpacity;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdMakeLayer : Command {
  explicit CmdMakeLayer(int id) : Command(id) {
  }

  int getType() const override {
    return LayerCommandType::MakeLayer;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetName : Command {
  std::string _name;

  CmdSetName(int id, const std::string& name) : Command(id), _name(name) {
  }

  int getType() const override {
    return LayerCommandType::setName;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetAlpha : Command {
  float _alpha;

  CmdSetAlpha(int id, float alpha) : Command(id), _alpha(alpha) {
  }

  int getType() const override {
    return LayerCommandType::setAlpha;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetBlendMode : Command {
  BlendMode _blendMode;

  CmdSetBlendMode(int id, BlendMode blendMode) : Command(id), _blendMode(blendMode) {
  }

  int getType() const override {
    return LayerCommandType::setBlendMode;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetPosition : Command {
  Point _position;

  CmdSetPosition(int id, const Point& position) : Command(id), _position(position) {
  }

  int getType() const override {
    return LayerCommandType::setPosition;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetMatrix : Command {
  Matrix _matrix;

  CmdSetMatrix(int id, const Matrix& matrix) : Command(id), _matrix(matrix) {
  }

  int getType() const override {
    return LayerCommandType::setMatrix;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetVisible : Command {
  bool _visible;

  CmdSetVisible(int id, bool visible) : Command(id), _visible(visible) {
  }

  int getType() const override {
    return LayerCommandType::setVisible;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetShouldRasterize : Command {
  bool _shouldRasterize;

  CmdSetShouldRasterize(int id, bool shouldRasterize)
      : Command(id), _shouldRasterize(shouldRasterize) {
  }

  int getType() const override {
    return LayerCommandType::setShouldRasterize;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetRasterizationScale : Command {
  float _scale;

  CmdSetRasterizationScale(int id, float scale) : Command(id), _scale(scale) {
  }

  int getType() const override {
    return LayerCommandType::setRasterizationScale;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetAllowsEdgeAntialiasing : Command {
  bool _allows;

  CmdSetAllowsEdgeAntialiasing(int id, bool allows) : Command(id), _allows(allows) {
  }

  int getType() const override {
    return LayerCommandType::setAllowsEdgeAntialiasing;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetAllowsGroupOpacity : Command {
  bool _allows;

  CmdSetAllowsGroupOpacity(int id, bool allows) : Command(id), _allows(allows) {
  }

  int getType() const override {
    return LayerCommandType::setAllowsGroupOpacity;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetFilters : Command {
  std::vector<int> _filter_ids;  // 将 LayerFilter 替换为 id

  CmdSetFilters(int id, const std::vector<int>& filter_ids) : Command(id), _filter_ids(filter_ids) {
  }

  int getType() const override {
    return LayerCommandType::setFilters;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetMask : Command {
  int _mask_id;  // 将 Layer 替换为 id

  CmdSetMask(int id, int mask_id) : Command(id), _mask_id(mask_id) {
  }

  int getType() const override {
    return LayerCommandType::setMask;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetScrollRect : Command {
  Rect _rect;

  CmdSetScrollRect(int id, const Rect& rect) : Command(id), _rect(rect) {
  }

  int getType() const override {
    return LayerCommandType::setScrollRect;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdAddChildAt : Command {
  int _child_id;  // 将 Layer 替换为 id
  int _index;

  CmdAddChildAt(int id, int child_id, int index) : Command(id), _child_id(child_id), _index(index) {
  }

  int getType() const override {
    return LayerCommandType::addChildAt;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdRemoveChildAt : Command {
  int _index;

  CmdRemoveChildAt(int id, int index) : Command(id), _index(index) {
  }

  int getType() const override {
    return LayerCommandType::removeChildAt;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdRemoveChildren : Command {
  int _beginIndex;
  int _endIndex;

  CmdRemoveChildren(int id, int beginIndex, int endIndex)
      : Command(id), _beginIndex(beginIndex), _endIndex(endIndex) {
  }

  int getType() const override {
    return LayerCommandType::removeChildren;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdRemoveFromParent : Command {
  CmdRemoveFromParent(int id) : Command(id) {
  }

  int getType() const override {
    return LayerCommandType::removeFromParent;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetChildIndex : Command {
  int _child_id;  // 将 Layer 替换为 id
  int _index;

  CmdSetChildIndex(int id, int child_id, int index)
      : Command(id), _child_id(child_id), _index(index) {
  }

  int getType() const override {
    return LayerCommandType::setChildIndex;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdReplaceChild : Command {
  int _oldChild_id;  // 将 Layer 替换为 id
  int _newChild_id;  // 将 Layer 替换为 id

  CmdReplaceChild(int id, int oldChild_id, int newChild_id)
      : Command(id), _oldChild_id(oldChild_id), _newChild_id(newChild_id) {
  }

  int getType() const override {
    return LayerCommandType::replaceChild;
  }

  bool doMerge(const Command& other) override;
  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};


}  // namespace tgfx
