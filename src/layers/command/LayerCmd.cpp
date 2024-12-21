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

std::unique_ptr<Command> LayerCmdFactory::MakeFrom(const nlohmann::json& json) {
  CommandType type = static_cast<CommandType>(json.at("type").get<int>());
  int id = json.at("id").get<int>();  // 提取 _id
  switch (type) {
    case CommandType::SetDefaultAllowsEdgeAntialiasing:
      return std::make_unique<CmdSetDefaultAllowsEdgeAntialiasing>(id,
                                                                   json.at("value").get<bool>());
    case CommandType::SetDefaultAllowsGroupOpacity:
      return std::make_unique<CmdSetDefaultAllowsGroupOpacity>(id, json.at("value").get<bool>());
    case CommandType::MakeLayer:
      return std::make_unique<CmdMakeLayer>(id);
    case CommandType::setName: {
      auto name = json.at("name").get<std::string>();
      return std::make_unique<CmdSetName>(id, name);
    }
    case CommandType::setAlpha:
      return std::make_unique<CmdSetAlpha>(id, json.at("alpha").get<float>());
    case CommandType::setBlendMode:
      return std::make_unique<CmdSetBlendMode>(
          id, static_cast<BlendMode>(json.at("blendMode").get<int>()));
    case CommandType::setPosition: {
      auto pos = json.at("position");
      Point position = Point::Make(pos[0].get<float>(), pos[1].get<float>());
      return std::make_unique<CmdSetPosition>(id, position);
    }
    case CommandType::setMatrix: {
      auto mat = json.at("matrix");
      Matrix matrix = Matrix::MakeAll(mat[0].get<float>(),  // scaleX
                                      mat[1].get<float>(),  // skewX
                                      mat[2].get<float>(),  // transX
                                      mat[3].get<float>(),  // skewY
                                      mat[4].get<float>(),  // scaleY
                                      mat[5].get<float>()   // transY
      );
      return std::make_unique<CmdSetMatrix>(id, matrix);
    }
    case CommandType::setVisible:
      return std::make_unique<CmdSetVisible>(id, json.at("visible").get<bool>());
    case CommandType::setShouldRasterize:
      return std::make_unique<CmdSetShouldRasterize>(id, json.at("shouldRasterize").get<bool>());
    case CommandType::setRasterizationScale:
      return std::make_unique<CmdSetRasterizationScale>(id, json.at("scale").get<float>());
    case CommandType::setAllowsEdgeAntialiasing:
      return std::make_unique<CmdSetAllowsEdgeAntialiasing>(id, json.at("allows").get<bool>());
    case CommandType::setAllowsGroupOpacity:
      return std::make_unique<CmdSetAllowsGroupOpacity>(id, json.at("allows").get<bool>());
    case CommandType::setFilters:
      return std::make_unique<CmdSetFilters>(id, json.at("filter_ids").get<std::vector<int>>());
    case CommandType::setMask:
      return std::make_unique<CmdSetMask>(id, json.at("mask_id").get<int>());
    case CommandType::setScrollRect: {
      auto rect = json.at("rect");
      Rect r = Rect::MakeXYWH(rect[0].get<float>(), rect[1].get<float>(), rect[2].get<float>(),
                              rect[3].get<float>());
      return std::make_unique<CmdSetScrollRect>(id, r);
    }
    case CommandType::addChildAt:
      return std::make_unique<CmdAddChildAt>(id, json.at("child_id").get<int>(),
                                             json.at("index").get<int>());
    case CommandType::removeChildAt:
      return std::make_unique<CmdRemoveChildAt>(id, json.at("index").get<int>());
    case CommandType::removeChildren:
      return std::make_unique<CmdRemoveChildren>(id, json.at("beginIndex").get<int>(),
                                                 json.at("endIndex").get<int>());
    case CommandType::removeFromParent:
      return std::make_unique<CmdRemoveFromParent>(id);
    case CommandType::setChildIndex:
      return std::make_unique<CmdSetChildIndex>(id, json.at("child_id").get<int>(),
                                                json.at("index").get<int>());
    case CommandType::replaceChild:
      return std::make_unique<CmdReplaceChild>(id, json.at("oldChild_id").get<int>(),
                                               json.at("newChild_id").get<int>());

    default:
      return nullptr;
  }
}

// ---------------- CmdSetDefaultAllowsEdgeAntialiasing ----------------

void CmdSetDefaultAllowsEdgeAntialiasing::execute(std::map<int, std::shared_ptr<Recordable>>&) {
  Layer::SetDefaultAllowsEdgeAntialiasing(_value);
}

nlohmann::json CmdSetDefaultAllowsEdgeAntialiasing::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"value", _value}};
}

bool CmdSetDefaultAllowsEdgeAntialiasing::doMerge(const Command& other) {
  _value = static_cast<const CmdSetDefaultAllowsEdgeAntialiasing&>(other)._value;
  return true;
}

// ---------------- CmdSetDefaultAllowsGroupOpacity ----------------

void CmdSetDefaultAllowsGroupOpacity::execute(std::map<int, std::shared_ptr<Recordable>>&) {
  Layer::SetDefaultAllowsGroupOpacity(_value);
}

nlohmann::json CmdSetDefaultAllowsGroupOpacity::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"value", _value}};
}

bool CmdSetDefaultAllowsGroupOpacity::doMerge(const Command& other) {
  _value = static_cast<const CmdSetDefaultAllowsGroupOpacity&>(other)._value;
  return true;
}

// ---------------- CmdMakeLayer ----------------

void CmdMakeLayer::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto layer = Layer::Make();
  layer->_uuid = _id;
  objMap[_id] = layer;
}

nlohmann::json CmdMakeLayer::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}};
}

bool CmdMakeLayer::doMerge(const Command&) {
  // 正常不可能make同一个对象(相同id、相同type）多次，可能上游出现错误了，打印一下
  std::cerr << "异常: CmdMakeLayer::doMerge, id = " << _id << std::endl;
  // 返回true，不需要重复make
  return true;
}

// ---------------- CmdSetName ----------------

void CmdSetName::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetName::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetName::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetName: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setName(_name);
}

nlohmann::json CmdSetName::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"name", _name}};
}

bool CmdSetName::doMerge(const Command& other) {
  _name = static_cast<const CmdSetName&>(other)._name;
  return true;
}

// ---------------- CmdSetAlpha ----------------

void CmdSetAlpha::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetAlpha::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetAlpha::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetAlpha: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setAlpha(_alpha);
}

nlohmann::json CmdSetAlpha::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"alpha", _alpha}};
}

bool CmdSetAlpha::doMerge(const Command& other) {
  _alpha = static_cast<const CmdSetAlpha&>(other)._alpha;
  return true;
}

// ---------------- CmdSetBlendMode ----------------

void CmdSetBlendMode::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetBlendMode::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetBlendMode::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetBlendMode: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setBlendMode(_blendMode);
}

nlohmann::json CmdSetBlendMode::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"blendMode", static_cast<int>(_blendMode)}};
}

bool CmdSetBlendMode::doMerge(const Command& other) {
  _blendMode = static_cast<const CmdSetBlendMode&>(other)._blendMode;
  return true;
}

// ---------------- CmdSetPosition ----------------

void CmdSetPosition::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetPosition::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetPosition::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetPosition: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setPosition(_position);
}

nlohmann::json CmdSetPosition::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"position", {_position.x, _position.y}}};
}

bool CmdSetPosition::doMerge(const Command& other) {
  _position = static_cast<const CmdSetPosition&>(other)._position;
  return true;
}

// ---------------- CmdSetMatrix ----------------

void CmdSetMatrix::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetMatrix::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetMatrix::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetMatrix: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setMatrix(_matrix);
}

nlohmann::json CmdSetMatrix::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          // 序列化 Matrix 的各个分量
          {"matrix",
           {_matrix.getScaleX(), _matrix.getSkewX(), _matrix.getTranslateX(), _matrix.getSkewY(),
            _matrix.getScaleY(), _matrix.getTranslateY()}}};
}

bool CmdSetMatrix::doMerge(const Command& other) {
  _matrix = static_cast<const CmdSetMatrix&>(other)._matrix;
  return true;
}

// ---------------- CmdSetVisible ----------------

void CmdSetVisible::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetVisible::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetVisible::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetVisible: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setVisible(_visible);
}

nlohmann::json CmdSetVisible::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"visible", _visible}};
}

bool CmdSetVisible::doMerge(const Command& other) {
  _visible = static_cast<const CmdSetVisible&>(other)._visible;
  return true;
}

// ---------------- CmdSetShouldRasterize ----------------

void CmdSetShouldRasterize::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetShouldRasterize::execute, objMap[" << _id << "] 未找到。"
              << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetShouldRasterize::execute, objMap[" << _id << "] 是空指针。"
              << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetShouldRasterize: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setShouldRasterize(_shouldRasterize);
}

nlohmann::json CmdSetShouldRasterize::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"shouldRasterize", _shouldRasterize}};
}

bool CmdSetShouldRasterize::doMerge(const Command& other) {
  _shouldRasterize = static_cast<const CmdSetShouldRasterize&>(other)._shouldRasterize;
  return true;
}

// ---------------- CmdSetRasterizationScale ----------------

void CmdSetRasterizationScale::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetRasterizationScale::execute, objMap[" << _id << "] 未找到。"
              << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetRasterizationScale::execute, objMap[" << _id << "] 是空指针。"
              << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetRasterizationScale: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setRasterizationScale(_scale);
}

nlohmann::json CmdSetRasterizationScale::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"scale", _scale}};
}

bool CmdSetRasterizationScale::doMerge(const Command& other) {
  _scale = static_cast<const CmdSetRasterizationScale&>(other)._scale;
  return true;
}

// ---------------- CmdSetAllowsEdgeAntialiasing ----------------

void CmdSetAllowsEdgeAntialiasing::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetAllowsEdgeAntialiasing::execute, objMap[" << _id << "] 未找到。"
              << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetAllowsEdgeAntialiasing::execute, objMap[" << _id << "] 是空指针。"
              << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetAllowsEdgeAntialiasing: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setAllowsEdgeAntialiasing(_allows);
}

nlohmann::json CmdSetAllowsEdgeAntialiasing::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"allows", _allows}};
}

bool CmdSetAllowsEdgeAntialiasing::doMerge(const Command& other) {
  _allows = static_cast<const CmdSetAllowsEdgeAntialiasing&>(other)._allows;
  return true;
}

// ---------------- CmdSetAllowsGroupOpacity ----------------

void CmdSetAllowsGroupOpacity::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetAllowsGroupOpacity::execute, objMap[" << _id << "] 未找到。"
              << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetAllowsGroupOpacity::execute, objMap[" << _id << "] 是空指针。"
              << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetAllowsGroupOpacity: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setAllowsGroupOpacity(_allows);
}

nlohmann::json CmdSetAllowsGroupOpacity::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"allows", _allows}};
}

bool CmdSetAllowsGroupOpacity::doMerge(const Command& other) {
  _allows = static_cast<const CmdSetAllowsGroupOpacity&>(other)._allows;
  return true;
}

// ---------------- CmdSetFilters ----------------

void CmdSetFilters::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetFilters::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetFilters::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  std::vector<std::shared_ptr<LayerFilter>> filters;
  for (int filter_id : _filter_ids) {
    auto filterIt = objMap.find(filter_id);
    if (filterIt != objMap.end()) {
      filters.push_back(std::static_pointer_cast<LayerFilter>(filterIt->second));
    }
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetFilters: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setFilters(filters);
}

nlohmann::json CmdSetFilters::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"filter_ids", _filter_ids}};
}

bool CmdSetFilters::doMerge(const Command& other) {
  _filter_ids = static_cast<const CmdSetFilters&>(other)._filter_ids;
  return true;
}

// ---------------- CmdSetMask ----------------

void CmdSetMask::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetMask::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetMask::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto maskIt = objMap.find(_mask_id);
  if (maskIt == objMap.end()) {
    std::cerr << "异常: CmdSetMask::execute, objMap[" << _mask_id << "] 未找到。" << std::endl;
    return;
  }
  if (!maskIt->second) {
    std::cerr << "异常: CmdSetMask::execute, objMap[" << _mask_id << "] 是空指针。" << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetMask: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setMask(std::static_pointer_cast<Layer>(maskIt->second));
}

nlohmann::json CmdSetMask::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"mask_id", _mask_id}};
}

bool CmdSetMask::doMerge(const Command& other) {
  _mask_id = static_cast<const CmdSetMask&>(other)._mask_id;
  return true;
}

// ---------------- CmdSetScrollRect ----------------

void CmdSetScrollRect::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetScrollRect::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdSetScrollRect::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdSetScrollRect: Layer Type: " << layer->TypeToString() << std::endl;
  layer->setScrollRect(_rect);
}

nlohmann::json CmdSetScrollRect::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"rect", {_rect.x(), _rect.y(), _rect.width(), _rect.height()}}};
}

bool CmdSetScrollRect::doMerge(const Command& other) {
  _rect = static_cast<const CmdSetScrollRect&>(other)._rect;
  return true;
}

// ---------------- CmdAddChildAt ----------------

void CmdAddChildAt::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto parentIt = objMap.find(_id);
  if (parentIt == objMap.end()) {
    std::cerr << "异常: CmdAddChildAt::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!parentIt->second) {
    std::cerr << "异常: CmdAddChildAt::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto childIt = objMap.find(_child_id);
  if (childIt == objMap.end()) {
    std::cerr << "异常: CmdAddChildAt::execute, objMap[" << _child_id << "] 未找到。" << std::endl;
    return;
  }
  if (!childIt->second) {
    std::cerr << "异常: CmdAddChildAt::execute, objMap[" << _child_id << "] 是空指针。"
              << std::endl;
    return;
  }
  auto parentLayer = std::static_pointer_cast<Layer>(parentIt->second);
  std::cout << "CmdAddChildAt: Parent Layer Type: " << parentLayer->TypeToString() << std::endl;
  auto childLayer = std::static_pointer_cast<Layer>(childIt->second);
  std::cout << "CmdAddChildAt: Child Layer Type: " << childLayer->TypeToString() << std::endl;
  if (_index > static_cast<int>(parentLayer->children().size())) {
    // 如果 index 超出范围，那说明前面有逻辑漏掉了，这样插入位置就不对了
    std::cerr << "异常: CmdAddChildAt::execute, index 超出范围。" << std::endl;
    return;
  }
  parentLayer->addChildAt(childLayer, _index);
}

nlohmann::json CmdAddChildAt::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"child_id", _child_id},
          {"index", _index}};
}

bool CmdAddChildAt::doMerge(const Command& other) {
  return Command::doMerge(other);
}

// ---------------- CmdRemoveChildAt ----------------

void CmdRemoveChildAt::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdRemoveChildAt::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdRemoveChildAt::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdRemoveChildAt: Layer Type: " << layer->TypeToString() << std::endl;
  layer->removeChildAt(_index);
}

nlohmann::json CmdRemoveChildAt::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"index", _index}};
}

bool CmdRemoveChildAt::doMerge(const Command& other) {
  return Command::doMerge(other);
}

// ---------------- CmdRemoveChildren ----------------

void CmdRemoveChildren::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdRemoveChildren::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdRemoveChildren::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdRemoveChildren: Layer Type: " << layer->TypeToString() << std::endl;
  layer->removeChildren(_beginIndex, _endIndex);
}

nlohmann::json CmdRemoveChildren::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"beginIndex", _beginIndex},
          {"endIndex", _endIndex}};
}

bool CmdRemoveChildren::doMerge(const Command& other) {
  return Command::doMerge(other);
}

// ---------------- CmdRemoveFromParent ----------------

void CmdRemoveFromParent::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdRemoveFromParent::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!it->second) {
    std::cerr << "异常: CmdRemoveFromParent::execute, objMap[" << _id << "] 是空指针。"
              << std::endl;
    return;
  }
  auto layer = std::static_pointer_cast<Layer>(it->second);
  std::cout << "CmdRemoveFromParent: Layer Type: " << layer->TypeToString() << std::endl;
  layer->removeFromParent();
}

nlohmann::json CmdRemoveFromParent::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}};
}

bool CmdRemoveFromParent::doMerge(const Command& other) {
  return Command::doMerge(other);
}

// ---------------- CmdSetChildIndex ----------------

void CmdSetChildIndex::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto parentIt = objMap.find(_id);
  if (parentIt == objMap.end()) {
    std::cerr << "异常: CmdSetChildIndex::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!parentIt->second) {
    std::cerr << "异常: CmdSetChildIndex::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto childIt = objMap.find(_child_id);
  if (childIt == objMap.end()) {
    std::cerr << "异常: CmdSetChildIndex::execute, objMap[" << _child_id << "] 未找到。"
              << std::endl;
    return;
  }
  if (!childIt->second) {
    std::cerr << "异常: CmdSetChildIndex::execute, objMap[" << _child_id << "] 是空指针。"
              << std::endl;
    return;
  }
  auto parentLayer = std::static_pointer_cast<Layer>(parentIt->second);
  std::cout << "CmdSetChildIndex: Parent Layer Type: " << parentLayer->TypeToString() << std::endl;
  auto childLayer = std::static_pointer_cast<Layer>(childIt->second);
  std::cout << "CmdSetChildIndex: Child Layer Type: " << childLayer->TypeToString() << std::endl;
  parentLayer->setChildIndex(childLayer, _index);
}

nlohmann::json CmdSetChildIndex::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"child_id", _child_id},
          {"index", _index}};
}

bool CmdSetChildIndex::doMerge(const Command& other) {
  return Command::doMerge(other);
}

// ---------------- CmdReplaceChild ----------------

void CmdReplaceChild::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto parentIt = objMap.find(_id);
  if (parentIt == objMap.end()) {
    std::cerr << "异常: CmdReplaceChild::execute, objMap[" << _id << "] 未找到。" << std::endl;
    return;
  }
  if (!parentIt->second) {
    std::cerr << "异常: CmdReplaceChild::execute, objMap[" << _id << "] 是空指针。" << std::endl;
    return;
  }
  auto oldChildIt = objMap.find(_oldChild_id);
  if (oldChildIt == objMap.end()) {
    std::cerr << "异常: CmdReplaceChild::execute, objMap[" << _oldChild_id << "] 未找到。"
              << std::endl;
    return;
  }
  if (!oldChildIt->second) {
    std::cerr << "异常: CmdReplaceChild::execute, objMap[" << _oldChild_id << "] 是空指针。"
              << std::endl;
    return;
  }
  auto newChildIt = objMap.find(_newChild_id);
  if (newChildIt == objMap.end()) {
    std::cerr << "异常: CmdReplaceChild::execute, objMap[" << _newChild_id << "] 未找到。"
              << std::endl;
    return;
  }
  if (!newChildIt->second) {
    std::cerr << "异常: CmdReplaceChild::execute, objMap[" << _newChild_id << "] 是空指针。"
              << std::endl;
    return;
  }
  auto parentLayer = std::static_pointer_cast<Layer>(parentIt->second);
  std::cout << "CmdReplaceChild: Parent Layer Type: " << parentLayer->TypeToString() << std::endl;
  auto oldChildLayer = std::static_pointer_cast<Layer>(oldChildIt->second);
  std::cout << "CmdReplaceChild: Old Child Layer Type: " << oldChildLayer->TypeToString() << std::endl;
  auto newChildLayer = std::static_pointer_cast<Layer>(newChildIt->second);
  std::cout << "CmdReplaceChild: New Child Layer Type: " << newChildLayer->TypeToString() << std::endl;
  parentLayer->replaceChild(oldChildLayer, newChildLayer);
}

nlohmann::json CmdReplaceChild::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"oldChild_id", _oldChild_id},
          {"newChild_id", _newChild_id}};
}

bool CmdReplaceChild::doMerge(const Command& other) {
  return Command::doMerge(other);
}

}  // namespace tgfx
