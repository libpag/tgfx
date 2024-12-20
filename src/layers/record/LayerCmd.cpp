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

#include "LayerCmd.h"
#include <iostream>

namespace tgfx {

void CmdSetDefaultAllowsEdgeAntialiasing::execute(std::map<int, std::shared_ptr<Recordable>>& ) {
  Layer::SetDefaultAllowsEdgeAntialiasing(_value);
}

nlohmann::json CmdSetDefaultAllowsEdgeAntialiasing::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"value", _value}};
}

void CmdSetDefaultAllowsGroupOpacity::execute(std::map<int, std::shared_ptr<Recordable>>& ) {
  Layer::SetDefaultAllowsGroupOpacity(_value);
}

nlohmann::json CmdSetDefaultAllowsGroupOpacity::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"value", _value}};
}

void CmdMakeLayer::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto layer = Layer::Make();
  layer->_uuid = _id;
  objMap[_id] = layer;
}

nlohmann::json CmdMakeLayer::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}};
}

void CmdSetName::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setName(_name);
}

nlohmann::json CmdSetName::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"name", _name}};
}

void CmdSetAlpha::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setAlpha(_alpha);
}

nlohmann::json CmdSetAlpha::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"alpha", _alpha}};
}

void CmdSetBlendMode::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setBlendMode(_blendMode);
}

nlohmann::json CmdSetBlendMode::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"blendMode", static_cast<int>(_blendMode)}};
}

void CmdSetPosition::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setPosition(_position);
}

nlohmann::json CmdSetPosition::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"position", {_position.x, _position.y}}};
}

nlohmann::json CmdSetMatrix::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          // 序列化 Matrix 的各个分量
          {"matrix",
           {_matrix.getScaleX(), _matrix.getSkewX(), _matrix.getTranslateX(), _matrix.getSkewY(),
            _matrix.getScaleY(), _matrix.getTranslateY()}}};
}

void CmdSetMatrix::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setMatrix(_matrix);
}

void CmdSetVisible::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setVisible(_visible);
}

nlohmann::json CmdSetVisible::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"visible", _visible}};
}

void CmdSetShouldRasterize::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setShouldRasterize(_shouldRasterize);
}

nlohmann::json CmdSetShouldRasterize::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"shouldRasterize", _shouldRasterize}};
}

void CmdSetRasterizationScale::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setRasterizationScale(_scale);
}

nlohmann::json CmdSetRasterizationScale::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"scale", _scale}};
}

void CmdSetAllowsEdgeAntialiasing::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setAllowsEdgeAntialiasing(_allows);
}

nlohmann::json CmdSetAllowsEdgeAntialiasing::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"allows", _allows}};
}

void CmdSetAllowsGroupOpacity::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setAllowsGroupOpacity(_allows);
}

nlohmann::json CmdSetAllowsGroupOpacity::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"allows", _allows}};
}

void CmdSetFilters::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::vector<std::shared_ptr<LayerFilter>> filters;
  for (int filter_id : _filter_ids) {
    auto filterIt = objMap.find(filter_id);
    if (filterIt != objMap.end()) {
      filters.push_back(std::static_pointer_cast<LayerFilter>(filterIt->second));
    }
  }
  std::static_pointer_cast<Layer>(it->second)->setFilters(filters);
}

nlohmann::json CmdSetFilters::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"filter_ids", _filter_ids}};
}

void CmdSetMask::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto maskIt = objMap.find(_mask_id);
  if (maskIt == objMap.end()) {
    std::cerr << "异常: objMap[" << _mask_id << "] 未找到。" << std::endl;
    return;
  }
  if (!maskIt->second) {
    std::cerr << "异常: objMap[" << _mask_id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setMask(std::static_pointer_cast<Layer>(maskIt->second));
}

nlohmann::json CmdSetMask::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"mask_id", _mask_id}};
}

void CmdSetScrollRect::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->setScrollRect(_rect);
}

nlohmann::json CmdSetScrollRect::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"rect", {_rect.x(), _rect.y(), _rect.width(), _rect.height()}}};
}

void CmdAddChildAt::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto parentIt = objMap.find(_id);
  if (parentIt == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!parentIt->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto childIt = objMap.find(_child_id);
  if (childIt == objMap.end()) {
    std::cerr << "异常: objMap[" << _child_id << "] 未找到。" << std::endl;
    return;
  }
  if (!childIt->second) {
    std::cerr << "异常: objMap[" << _child_id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(parentIt->second)
      ->addChildAt(std::static_pointer_cast<Layer>(childIt->second), _index);
}

nlohmann::json CmdAddChildAt::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"child_id", _child_id},
          {"index", _index}};
}

void CmdRemoveChildAt::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->removeChildAt(_index);
}

nlohmann::json CmdRemoveChildAt::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"index", _index}};
}

void CmdRemoveChildren::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->removeChildren(_beginIndex, _endIndex);
}

nlohmann::json CmdRemoveChildren::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"beginIndex", _beginIndex},
          {"endIndex", _endIndex}};
}

void CmdRemoveFromParent::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(it->second)->removeFromParent();
}

nlohmann::json CmdRemoveFromParent::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}};
}

void CmdSetChildIndex::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto parentIt = objMap.find(_id);
  if (parentIt == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!parentIt->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto childIt = objMap.find(_child_id);
  if (childIt == objMap.end()) {
    std::cerr << "异常: objMap[" << _child_id << "] 未找到。" << std::endl;
    return;
  }
  if (!childIt->second) {
    std::cerr << "异常: objMap[" << _child_id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(parentIt->second)
      ->setChildIndex(std::static_pointer_cast<Layer>(childIt->second), _index);
}

nlohmann::json CmdSetChildIndex::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"child_id", _child_id},
          {"index", _index}};
}

void CmdReplaceChild::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto parentIt = objMap.find(_id);
  if (parentIt == objMap.end()) {
    std::cerr << "异常: objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!parentIt->second) {
    std::cerr << "异常: objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto oldChildIt = objMap.find(_oldChild_id);
  if (oldChildIt == objMap.end()) {
    std::cerr << "异常: objMap[" << _oldChild_id << "] 未找到。" << std::endl;
    return;
  }
  if (!oldChildIt->second) {
    std::cerr << "异常: objMap[" << _oldChild_id << "] 是空指针。" << std::endl;
    return;
  }
  auto newChildIt = objMap.find(_newChild_id);
  if (newChildIt == objMap.end()) {
    std::cerr << "异常: objMap[" << _newChild_id << "] 未找到。" << std::endl;
    return;
  }
  if (!newChildIt->second) {
    std::cerr << "异常: objMap[" << _newChild_id << "] 是空指针。" << std::endl;
    return;
  }
  std::static_pointer_cast<Layer>(parentIt->second)
      ->replaceChild(std::static_pointer_cast<Layer>(oldChildIt->second),
                     std::static_pointer_cast<Layer>(newChildIt->second));
}

nlohmann::json CmdReplaceChild::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"oldChild_id", _oldChild_id},
          {"newChild_id", _newChild_id}};
}
}  // namespace tgfx
