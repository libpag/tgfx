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

#include "Commands.h"
#include "CommandType.h"

namespace tgfx {
class ShapeStyleCmdFactory {
 public:
  static std::unique_ptr<Command> MakeFrom(const nlohmann::json& json);
};

struct CmdMakeSolidColor : Command {
  Color _color;
  CmdMakeSolidColor(int id, const Color& color) : Command(id), _color(color) {
  }

  int getType() const override {
    return ShapeStyleCommandType::MakeSolidColor;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetSolidColor : Command {
  Color _color;

  CmdSetSolidColor(int id, const Color& color) : Command(id), _color(color) {
  }

  int getType() const override {
    return ShapeStyleCommandType::setColor;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

}  // namespace tgfx
