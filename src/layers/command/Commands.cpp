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
#include <iostream>
#include "LayerCmd.h"
#include "ShapeLayerCmd.h"
#include "SolidLayerCmd.h"
#include "tgfx/core/Color.h"

int getNextCommandIndex() {
  static int counter = 1000;
  return counter++;
}

namespace tgfx {

nlohmann::json Command::ColorToJson(const Color& color) {
  return {{"red", color.red}, {"green", color.green}, {"blue", color.blue}, {"alpha", color.alpha}};
}

Color Command::JsonToColor(const nlohmann::json& json) {
  return Color{json.value("red", 0.0f), json.value("green", 0.0f), json.value("blue", 0.0f),
               json.value("alpha", 1.0f)};
}

std::unique_ptr<Command> Command::MakeFrom(const nlohmann::json& json) {

  std::unique_ptr<Command> command = LayerCmdFactory::MakeFrom(json);
  if (command != nullptr) {
    return command;
  }

  command = ShapeLayerCmdFactory::MakeFrom(json);
  if (command != nullptr) {
    return command;
  }

  command = SolidLayerCmdFactory::MakeFrom(json);
  if (command != nullptr) {
    return command;
  }
  std::cerr << "不支持的命令, json is " << json.dump() << std::endl;
  return nullptr;
}
bool Command::merge(const Command& other) {
  // id 不同，或者 type 不同，不可合并
  if (other._id != _id || other.getType() != getType()) {
    return false;
  }
  return doMerge(other);
}

bool Command::doMerge(const Command&) {
  // 默认不可合并，看子类
  return false;
}

}  // namespace tgfx
