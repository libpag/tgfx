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

#include <tgfx/core/Path.h>
#include <tgfx/core/Shape.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/record/Recordable.h>
#include <nlohmann/json.hpp>
#include "Commands.h"
#include "CommandType.h"


namespace tgfx {

class SolidLayerCmdFactory {
 public:
  static std::unique_ptr<Command> MakeFrom(const nlohmann::json& json);
};

struct CmdMakeSolidLayer : Command {
  explicit CmdMakeSolidLayer(int id) : Command(id) {
  }

  int getType() const override {
    return SolidLayerCommandType::MakeSolidLayer;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetWidth : Command {
  float _width;

  CmdSetWidth(int id, float width) : Command(id), _width(width) {
  }

  int getType() const override {
    return SolidLayerCommandType::setWidth;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetHeight : Command {
  float _height;

  CmdSetHeight(int id, float height) : Command(id), _height(height) {
  }

  int getType() const override {
    return SolidLayerCommandType::setHeight;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetRadiusX : Command {
  float _radiusX;

  CmdSetRadiusX(int id, float radiusX) : Command(id), _radiusX(radiusX) {
  }

  int getType() const override {
    return SolidLayerCommandType::setRadiusX;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetRadiusY : Command {
  float _radiusY;

  CmdSetRadiusY(int id, float radiusY) : Command(id), _radiusY(radiusY) {
  }

  int getType() const override {
    return SolidLayerCommandType::setRadiusY;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetColor : Command {
  Color _color;

  CmdSetColor(int id, const Color& color) : Command(id), _color(color) {
  }

  int getType() const override {
    return SolidLayerCommandType::setColor;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

// ------------------------ SolidLayerRecorder end ------------------------

}  // namespace tgfx
