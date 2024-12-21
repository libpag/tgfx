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
#include <tgfx/core/Path.h>
#include <tgfx/core/Shape.h>
#include <tgfx/layers/ShapeLayer.h>
#include <tgfx/layers/record/Recordable.h>
#include <nlohmann/json.hpp>

namespace tgfx {


class ShapeLayerCmdFactory {
public:
  static std::unique_ptr<Command> MakeFrom(const nlohmann::json& json);
};

struct CmdMakeShapeLayer : Command {
  explicit CmdMakeShapeLayer(int id) : Command(id) {}

  CommandType getType() const override {
    return CommandType::MakeShapeLayer;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetPath : Command {
  Path _path;

  CmdSetPath(int id, const Path& path) : Command(id), _path(path) {}

  CommandType getType() const override {
    return CommandType::setPath;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetShape : Command {
  std::shared_ptr<Shape> _shape;

  CmdSetShape(int id, const std::shared_ptr<Shape>& shape) : Command(id), _shape(shape) {}

  CommandType getType() const override {
    return CommandType::setShape;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetFillStyle : Command {
  int _styleId;

  CmdSetFillStyle(int id, int styleId) : Command(id), _styleId(styleId) {}

  CommandType getType() const override {
    return CommandType::setFillStyle;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetStrokeStyle : Command {
  int _styleId;

  CmdSetStrokeStyle(int id, int styleId) : Command(id), _styleId(styleId) {}

  CommandType getType() const override {
    return CommandType::setStrokeStyle;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetLineCap : Command {
  LineCap _cap;

  CmdSetLineCap(int id, LineCap cap) : Command(id), _cap(cap) {}

  CommandType getType() const override {
    return CommandType::setLineCap;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetLineJoin : Command {
  LineJoin _join;

  CmdSetLineJoin(int id, LineJoin join) : Command(id), _join(join) {}

  CommandType getType() const override {
    return CommandType::setLineJoin;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetMiterLimit : Command {
  float _limit;

  CmdSetMiterLimit(int id, float limit) : Command(id), _limit(limit) {}

  CommandType getType() const override {
    return CommandType::setMiterLimit;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetLineWidth : Command {
  float _width;

  CmdSetLineWidth(int id, float width) : Command(id), _width(width) {}

  CommandType getType() const override {
    return CommandType::setLineWidth;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetLineDashPattern : Command {
  std::vector<float> _pattern;

  CmdSetLineDashPattern(int id, const std::vector<float>& pattern) : Command(id), _pattern(pattern) {}

  CommandType getType() const override {
    return CommandType::setLineDashPattern;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetLineDashPhase : Command {
  float _phase;

  CmdSetLineDashPhase(int id, float phase) : Command(id), _phase(phase) {}

  CommandType getType() const override {
    return CommandType::setLineDashPhase;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetStrokeStart : Command {
  float _start;

  CmdSetStrokeStart(int id, float start) : Command(id), _start(start) {}

  CommandType getType() const override {
    return CommandType::setStrokeStart;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetStrokeEnd : Command {
  float _end;

  CmdSetStrokeEnd(int id, float end) : Command(id), _end(end) {}

  CommandType getType() const override {
    return CommandType::setStrokeEnd;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

struct CmdSetStrokeAlign : Command {
  StrokeAlign _align;

  CmdSetStrokeAlign(int id, StrokeAlign align) : Command(id), _align(align) {}

  CommandType getType() const override {
    return CommandType::setStrokeAlign;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
  bool doMerge(const Command& other) override;
};

}  // namespace tgfx
