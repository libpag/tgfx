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

#include "Commands.h"
#include <tgfx/layers/Layer.h>

namespace tgfx {

void CmdSetDefaultAllowsEdgeAntialiasing::execute(std::map<int, std::shared_ptr<Recordable>>&) {
  Layer::SetDefaultAllowsEdgeAntialiasing(_value);
}

nlohmann::json CmdSetDefaultAllowsEdgeAntialiasing::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"value", _value}};
}

void CmdSetDefaultAllowsGroupOpacity::execute(std::map<int, std::shared_ptr<Recordable>>&) {
  Layer::SetDefaultAllowsGroupOpacity(_value);
}

nlohmann::json CmdSetDefaultAllowsGroupOpacity::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"value", _value}};
}

void CmdMakeLayer::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto layer = Layer::Make();
  layer->_uuid = _id;
  objMap[_id] = layer;
}

nlohmann::json CmdMakeLayer::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}};
}

void CmdSetAlpha::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setAlpha(_alpha);
  }
}

nlohmann::json CmdSetAlpha::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"alpha", _alpha}};
}

void CmdSetBlendMode::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setBlendMode(_blendMode);
  }
}

nlohmann::json CmdSetBlendMode::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"blendMode", static_cast<int>(_blendMode)}};
}

void CmdSetPosition::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setPosition(_position);
  }
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
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setMatrix(_matrix);
  }
}

void CmdSetVisible::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setVisible(_visible);
  }
}

nlohmann::json CmdSetVisible::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"visible", _visible}};
}

void CmdSetShouldRasterize::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setShouldRasterize(_shouldRasterize);
  }
}

nlohmann::json CmdSetShouldRasterize::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"shouldRasterize", _shouldRasterize}};
}

void CmdSetRasterizationScale::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setRasterizationScale(_scale);
  }
}

nlohmann::json CmdSetRasterizationScale::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"scale", _scale}};
}

void CmdSetAllowsEdgeAntialiasing::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setAllowsEdgeAntialiasing(_allows);
  }
}

nlohmann::json CmdSetAllowsEdgeAntialiasing::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"allows", _allows}};
}

void CmdSetAllowsGroupOpacity::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setAllowsGroupOpacity(_allows);
  }
}

nlohmann::json CmdSetAllowsGroupOpacity::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"allows", _allows}};
}

void CmdSetFilters::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::vector<std::shared_ptr<LayerFilter>> filters;
    for (int filter_id : _filter_ids) {
      auto filterIt = objMap.find(filter_id);
      if (filterIt != objMap.end()) {
        filters.push_back(std::static_pointer_cast<LayerFilter>(filterIt->second));
      }
    }
    static_cast<Layer*>(it->second.get())->setFilters(filters);
  }
}

nlohmann::json CmdSetFilters::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"filter_ids", _filter_ids}};
}

void CmdSetMask::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  auto maskIt = objMap.find(_mask_id);
  if (it != objMap.end() && maskIt != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setMask(std::static_pointer_cast<Layer>(maskIt->second));
  }
}

nlohmann::json CmdSetMask::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"mask_id", _mask_id}};
}

void CmdSetScrollRect::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setScrollRect(_rect);
  }
}

nlohmann::json CmdSetScrollRect::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"rect", {_rect.x(), _rect.y(), _rect.width(), _rect.height()}}};
}

void CmdAddChildAt::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto parentIt = objMap.find(_id);
  auto childIt = objMap.find(_child_id);
  if (parentIt != objMap.end() && childIt != objMap.end()) {
    static_cast<Layer*>(parentIt->second.get())
        ->addChildAt(std::static_pointer_cast<Layer>(childIt->second), _index);
  }
}

nlohmann::json CmdAddChildAt::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"child_id", _child_id},
          {"index", _index}};
}

void CmdRemoveChildAt::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->removeChildAt(_index);
  }
}

nlohmann::json CmdRemoveChildAt::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"index", _index}};
}

void CmdRemoveChildren::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->removeChildren(_beginIndex, _endIndex);
  }
}

nlohmann::json CmdRemoveChildren::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"beginIndex", _beginIndex},
          {"endIndex", _endIndex}};
}

void CmdRemoveFromParent::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->removeFromParent();
  }
}

nlohmann::json CmdRemoveFromParent::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}};
}

void CmdSetChildIndex::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto parentIt = objMap.find(_id);
  auto childIt = objMap.find(_child_id);
  if (parentIt != objMap.end() && childIt != objMap.end()) {

    static_cast<Layer*>(parentIt->second.get())
        ->setChildIndex(std::static_pointer_cast<Layer>(childIt->second), _index);
  }
}

nlohmann::json CmdSetChildIndex::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"child_id", _child_id},
          {"index", _index}};
}

void CmdReplaceChild::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto parentIt = objMap.find(_id);
  auto oldChildIt = objMap.find(_oldChild_id);
  auto newChildIt = objMap.find(_newChild_id);
  if (parentIt != objMap.end() && oldChildIt != objMap.end() && newChildIt != objMap.end()) {
    static_cast<Layer*>(parentIt->second.get())
        ->replaceChild(std::static_pointer_cast<Layer>(oldChildIt->second),
                       std::static_pointer_cast<Layer>(newChildIt->second));
  }
}

nlohmann::json CmdReplaceChild::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"oldChild_id", _oldChild_id},
          {"newChild_id", _newChild_id}};
}

std::unique_ptr<Command> Command::MakeFrom(const nlohmann::json& json) {
  CommandType type = static_cast<CommandType>(json.at("type").get<int>());
  switch (type) {
    case CommandType::SetDefaultAllowsEdgeAntialiasing:
      return std::make_unique<CmdSetDefaultAllowsEdgeAntialiasing>(json.at("value").get<bool>());
    case CommandType::SetDefaultAllowsGroupOpacity:
      return std::make_unique<CmdSetDefaultAllowsGroupOpacity>(json.at("value").get<bool>());
    case CommandType::MakeLayer:
      return std::make_unique<CmdMakeLayer>(json.at("id").get<int>());
    case CommandType::setAlpha:
      return std::make_unique<CmdSetAlpha>(json.at("id").get<int>(), json.at("alpha").get<float>());
    case CommandType::setBlendMode:
      return std::make_unique<CmdSetBlendMode>(
          json.at("id").get<int>(), static_cast<BlendMode>(json.at("blendMode").get<int>()));
    case CommandType::setPosition: {
      auto pos = json.at("position");
      Point position = Point::Make(pos[0].get<float>(), pos[1].get<float>());
      return std::make_unique<CmdSetPosition>(json.at("id").get<int>(), position);
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
      return std::make_unique<CmdSetMatrix>(json.at("id").get<int>(), matrix);
    }
    case CommandType::setVisible:
      return std::make_unique<CmdSetVisible>(json.at("id").get<int>(),
                                             json.at("visible").get<bool>());
    case CommandType::setShouldRasterize:
      return std::make_unique<CmdSetShouldRasterize>(json.at("id").get<int>(),
                                                     json.at("shouldRasterize").get<bool>());
    case CommandType::setRasterizationScale:
      return std::make_unique<CmdSetRasterizationScale>(json.at("id").get<int>(),
                                                        json.at("scale").get<float>());
    case CommandType::setAllowsEdgeAntialiasing:
      return std::make_unique<CmdSetAllowsEdgeAntialiasing>(json.at("id").get<int>(),
                                                            json.at("allows").get<bool>());
    case CommandType::setAllowsGroupOpacity:
      return std::make_unique<CmdSetAllowsGroupOpacity>(json.at("id").get<int>(),
                                                        json.at("allows").get<bool>());
    case CommandType::setFilters:
      return std::make_unique<CmdSetFilters>(json.at("id").get<int>(),
                                             json.at("filter_ids").get<std::vector<int>>());
    case CommandType::setMask:
      return std::make_unique<CmdSetMask>(json.at("id").get<int>(), json.at("mask_id").get<int>());
    case CommandType::setScrollRect: {
      auto rect = json.at("rect");
      Rect r = Rect::MakeXYWH(rect[0].get<float>(), rect[1].get<float>(), rect[2].get<float>(),
                              rect[3].get<float>());
      return std::make_unique<CmdSetScrollRect>(json.at("id").get<int>(), r);
    }
    case CommandType::addChildAt:
      return std::make_unique<CmdAddChildAt>(
          json.at("id").get<int>(), json.at("child_id").get<int>(), json.at("index").get<int>());
    case CommandType::removeChildAt:
      return std::make_unique<CmdRemoveChildAt>(json.at("id").get<int>(),
                                                json.at("index").get<int>());
    case CommandType::removeChildren:
      return std::make_unique<CmdRemoveChildren>(json.at("id").get<int>(),
                                                 json.at("beginIndex").get<int>(),
                                                 json.at("endIndex").get<int>());
    case CommandType::removeFromParent:
      return std::make_unique<CmdRemoveFromParent>(json.at("id").get<int>());
    case CommandType::setChildIndex:
      return std::make_unique<CmdSetChildIndex>(
          json.at("id").get<int>(), json.at("child_id").get<int>(), json.at("index").get<int>());
    case CommandType::replaceChild:
      return std::make_unique<CmdReplaceChild>(json.at("id").get<int>(),
                                               json.at("oldChild_id").get<int>(),
                                               json.at("newChild_id").get<int>());
    default:
      throw std::invalid_argument("Unknown CommandType");
  }
}

}  // namespace tgfx
