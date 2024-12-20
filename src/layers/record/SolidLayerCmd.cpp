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

#include "SolidLayerCmd.h"
#include "tgfx/layers/SolidLayer.h"
#include <iostream>

namespace tgfx {

void CmdMakeSolidLayer::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  objMap[_id] = SolidLayer::Make();
}

nlohmann::json CmdMakeSolidLayer::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}};
}

bool CmdMakeSolidLayer::doMerge(const Command&) {
  // 通常不可能多次创建同一个对象（相同id和type），记录异常
  std::cerr << "异常: CmdMakeSolidLayer::doMerge, id = " << _id << std::endl;
  // 返回true，避免重复创建
  return true;
}

void CmdSetWidth::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<SolidLayer>(it->second)->setWidth(_width);
  }
}

nlohmann::json CmdSetWidth::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"width", _width}};
}

bool CmdSetWidth::doMerge(const Command& other) {
  _width = static_cast<const CmdSetWidth&>(other)._width;
  return true;
}

void CmdSetHeight::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<SolidLayer>(it->second)->setHeight(_height);
  }
}

nlohmann::json CmdSetHeight::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"height", _height}};
}

bool CmdSetHeight::doMerge(const Command& other) {
  _height = static_cast<const CmdSetHeight&>(other)._height;
  return true;
}

void CmdSetRadiusX::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<SolidLayer>(it->second)->setRadiusX(_radiusX);
  }
}

nlohmann::json CmdSetRadiusX::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"radiusX", _radiusX}};
}

bool CmdSetRadiusX::doMerge(const Command& other) {
  _radiusX = static_cast<const CmdSetRadiusX&>(other)._radiusX;
  return true;
}

void CmdSetRadiusY::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<SolidLayer>(it->second)->setRadiusY(_radiusY);
  }
}

nlohmann::json CmdSetRadiusY::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"radiusY", _radiusY}};
}

bool CmdSetRadiusY::doMerge(const Command& other) {
  _radiusY = static_cast<const CmdSetRadiusY&>(other)._radiusY;
  return true;
}

void CmdSetColor::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<SolidLayer>(it->second)->setColor(_color);
  }
}

nlohmann::json CmdSetColor::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"color", Command::ColorToJson(_color)}};
}

bool CmdSetColor::doMerge(const Command& other) {
  _color = static_cast<const CmdSetColor&>(other)._color;
  return true;
}

}  // namespace tgfx
