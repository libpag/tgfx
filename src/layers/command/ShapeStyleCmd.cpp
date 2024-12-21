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

#include "ShapeStyleCmd.h"
#include <iostream>
#include "tgfx/layers/SolidColor.h"

namespace tgfx {

std::unique_ptr<Command> ShapeStyleCmdFactory::MakeFrom(const nlohmann::json& json) {
  int type = json.at("type").get<int>();
  int id = json.at("id").get<int>();
  if (type == ShapeStyleCommandType::MakeSolidColor) {
    return std::make_unique<CmdMakeSolidColor>(id, Command::JsonToColor(json.at("color")));
  }
  if (type == ShapeStyleCommandType::setColor) {
    return std::make_unique<CmdSetSolidColor>(id, Command::JsonToColor(json.at("color")));
  }
  return nullptr;
}

// ---------------- MakeSolidColor ----------------
void CmdMakeSolidColor::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  objMap[_id] = SolidColor::Make(_color);
}

nlohmann::json CmdMakeSolidColor::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"color", Command::ColorToJson(_color)}};
}

bool CmdMakeSolidColor::doMerge(const Command&) {
  std::cerr << "异常: MakeSolidColor::doMerge, id = " << _id << std::endl;
  return true;
}

// ---------------- CmdSetSolidColor ----------------
void CmdSetSolidColor::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetSolidColor::execute 找不到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<SolidColor>(it->second)->setColor(_color);
}

nlohmann::json CmdSetSolidColor::toJson() const {
  return {{"type", static_cast<int>(getType())},
          {"id", _id},
          {"color", Command::ColorToJson(_color)}};
}

bool CmdSetSolidColor::doMerge(const Command& other) {
  _color = static_cast<const CmdSetSolidColor&>(other)._color;
  return true;
}

}  // namespace tgfx