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


// ------------------------ ShapeLayer begin ------------------------

struct CmdMakeShapeLayer : Command {
  int _id;

  explicit CmdMakeShapeLayer(int id) : _id(id) {}

  CommandType getType() const override {
    return CommandType::MakeShapeLayer;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetPath : Command {
  int _id;
  Path _path;

  CmdSetPath(int id, const Path& path) : _id(id), _path(path) {}

  CommandType getType() const override {
    return CommandType::setPath;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetShape : Command {
  int _id;
  std::shared_ptr<Shape> _shape;

  CmdSetShape(int id, const std::shared_ptr<Shape>& shape) : _id(id), _shape(shape) {}

  CommandType getType() const override {
    return CommandType::setShape;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetFillStyle : Command {
  int _id;
  int _styleId;

  CmdSetFillStyle(int id, int styleId) : _id(id), _styleId(styleId) {}

  CommandType getType() const override {
    return CommandType::setFillStyle;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetStrokeStyle : Command {
  int _id;
  int _styleId;

  CmdSetStrokeStyle(int id, int styleId) : _id(id), _styleId(styleId) {}

  CommandType getType() const override {
    return CommandType::setStrokeStyle;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetLineCap : Command {
  int _id;
  LineCap _cap;

  CmdSetLineCap(int id, LineCap cap) : _id(id), _cap(cap) {}

  CommandType getType() const override {
    return CommandType::setLineCap;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetLineJoin : Command {
  int _id;
  LineJoin _join;

  CmdSetLineJoin(int id, LineJoin join) : _id(id), _join(join) {}

  CommandType getType() const override {
    return CommandType::setLineJoin;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetMiterLimit : Command {
  int _id;
  float _limit;

  CmdSetMiterLimit(int id, float limit) : _id(id), _limit(limit) {}

  CommandType getType() const override {
    return CommandType::setMiterLimit;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetLineWidth : Command {
  int _id;
  float _width;

  CmdSetLineWidth(int id, float width) : _id(id), _width(width) {}

  CommandType getType() const override {
    return CommandType::setLineWidth;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetLineDashPattern : Command {
  int _id;
  std::vector<float> _pattern;

  CmdSetLineDashPattern(int id, const std::vector<float>& pattern) : _id(id), _pattern(pattern) {}

  CommandType getType() const override {
    return CommandType::setLineDashPattern;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetLineDashPhase : Command {
  int _id;
  float _phase;

  CmdSetLineDashPhase(int id, float phase) : _id(id), _phase(phase) {}

  CommandType getType() const override {
    return CommandType::setLineDashPhase;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetStrokeStart : Command {
  int _id;
  float _start;

  CmdSetStrokeStart(int id, float start) : _id(id), _start(start) {}

  CommandType getType() const override {
    return CommandType::setStrokeStart;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetStrokeEnd : Command {
  int _id;
  float _end;

  CmdSetStrokeEnd(int id, float end) : _id(id), _end(end) {}

  CommandType getType() const override {
    return CommandType::setStrokeEnd;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

struct CmdSetStrokeAlign : Command {
  int _id;
  StrokeAlign _align;

  CmdSetStrokeAlign(int id, StrokeAlign align) : _id(id), _align(align) {}

  CommandType getType() const override {
    return CommandType::setStrokeAlign;
  }

  void execute(std::map<int, std::shared_ptr<Recordable>>& objMap) override;
  nlohmann::json toJson() const override;
};

}  // namespace tgfx
