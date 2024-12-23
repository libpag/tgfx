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

#include "ShapeLayerCmd.h"
#include <iostream>
#include "tgfx/layers/ShapeLayer.h"

namespace tgfx {

std::unique_ptr<Command> ShapeLayerCmdFactory::MakeFrom(const nlohmann::json& json) {

  int type = json.at("type").get<int>();
  int id = json.at("id").get<int>();  // 提取 _id
  if (type == ShapeLayerCommandType::MakeShapeLayer) {
    return std::make_unique<CmdMakeShapeLayer>(id);
  }
  if (type == ShapeLayerCommandType::setPath) {
    Path path;
    path.fromString(json.at("path"));
    return std::make_unique<CmdSetPath>(id, path);
  }
  if (type == ShapeLayerCommandType::setShape) {
    auto shapeJson = json.at("shape");
    std::shared_ptr<Shape> shape = Shape::FromJson(shapeJson.dump());
    return std::make_unique<CmdSetShape>(id, shape);
  }
  if (type == ShapeLayerCommandType::setFillStyle) {
    return std::make_unique<CmdSetFillStyle>(id, json.at("styleId").get<int>());
  }
  if (type == ShapeLayerCommandType::setStrokeStyle) {
    return std::make_unique<CmdSetStrokeStyle>(id, json.at("styleId").get<int>());
  }
  if (type == ShapeLayerCommandType::setLineCap) {
    return std::make_unique<CmdSetLineCap>(id, static_cast<LineCap>(json.at("cap").get<int>()));
  }
  if (type == ShapeLayerCommandType::setLineJoin) {
    return std::make_unique<CmdSetLineJoin>(id, static_cast<LineJoin>(json.at("join").get<int>()));
  }
  if (type == ShapeLayerCommandType::setMiterLimit) {
    return std::make_unique<CmdSetMiterLimit>(id, json.at("limit").get<float>());
  }
  if (type == ShapeLayerCommandType::setLineWidth) {
    return std::make_unique<CmdSetLineWidth>(id, json.at("width").get<float>());
  }
  if (type == ShapeLayerCommandType::setLineDashPattern) {
    return std::make_unique<CmdSetLineDashPattern>(id,
                                                   json.at("pattern").get<std::vector<float>>());
  }
  if (type == ShapeLayerCommandType::setLineDashPhase) {
    return std::make_unique<CmdSetLineDashPhase>(id, json.at("phase").get<float>());
  }
  if (type == ShapeLayerCommandType::setStrokeStart) {
    return std::make_unique<CmdSetStrokeStart>(id, json.at("start").get<float>());
  }
  if (type == ShapeLayerCommandType::setStrokeEnd) {
    return std::make_unique<CmdSetStrokeEnd>(id, json.at("end").get<float>());
  }
  if (type == ShapeLayerCommandType::setStrokeAlign) {
    return std::make_unique<CmdSetStrokeAlign>(
        id, static_cast<StrokeAlign>(json.at("align").get<int>()));
  }
  return nullptr;
  // ...existing code...
}

// ===== CmdMakeShapeLayer =====
void CmdMakeShapeLayer::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  objMap[_id] = ShapeLayer::Make();
}

nlohmann::json CmdMakeShapeLayer::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}};
}

bool CmdMakeShapeLayer::doMerge(const Command&) {
  // 正常不可能make同一个对象(相同id、相同type）多次，可能上游出现错误了，打印一下
  std::cerr << "异常: CmdMakeShapeLayer::doMerge, id = " << _id << std::endl;
  // 返回true，不需要重复make
  return true;
}

// ===== CmdSetPath =====
void CmdSetPath::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetPath::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setPath(_path);
}

nlohmann::json CmdSetPath::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"path", _path.toString()}};
}

bool CmdSetPath::doMerge(const Command& other) {
  _path = static_cast<const CmdSetPath&>(other)._path;
  return true;
}

// ===== CmdSetShape =====
void CmdSetShape::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetShape::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setShape(_shape);
}

nlohmann::json CmdSetShape::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"shape", _shape->toJson()}};
}

bool CmdSetShape::doMerge(const Command& other) {
  _shape = static_cast<const CmdSetShape&>(other)._shape;
  return true;
}

// ===== CmdSetFillStyle =====
void CmdSetFillStyle::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetFillStyle::execute 未找到 id = " << _id << std::endl;
    return;
  }
  auto styleIt = objMap.find(_styleId);
  if (styleIt == objMap.end()) {
    std::cerr << "异常: CmdSetFillStyle::execute 未找到 styleId = " << _styleId << std::endl;
    return;
  }
  auto style = std::static_pointer_cast<ShapeStyle>(styleIt->second);
  std::static_pointer_cast<ShapeLayer>(it->second)->setFillStyle(style);
}

nlohmann::json CmdSetFillStyle::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"styleId", _styleId}};
}

bool CmdSetFillStyle::doMerge(const Command& other) {
  _styleId = static_cast<const CmdSetFillStyle&>(other)._styleId;
  return true;
}

// ===== CmdSetStrokeStyle =====
void CmdSetStrokeStyle::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetStrokeStyle::execute 未找到 id = " << _id << std::endl;
    return;
  }
  auto styleIt = objMap.find(_styleId);
  if (styleIt == objMap.end()) {
    std::cerr << "异常: CmdSetStrokeStyle::execute 未找到 styleId = " << _styleId << std::endl;
    return;
  }
  auto style = std::static_pointer_cast<ShapeStyle>(styleIt->second);
  std::static_pointer_cast<ShapeLayer>(it->second)->setStrokeStyle(style);
}

nlohmann::json CmdSetStrokeStyle::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"styleId", _styleId}};
}

bool CmdSetStrokeStyle::doMerge(const Command& other) {
  _styleId = static_cast<const CmdSetStrokeStyle&>(other)._styleId;
  return true;
}

// ===== CmdSetLineCap =====
void CmdSetLineCap::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetLineCap::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setLineCap(_cap);
}

nlohmann::json CmdSetLineCap::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"cap", static_cast<int>(_cap)}};
}

bool CmdSetLineCap::doMerge(const Command& other) {
  _cap = static_cast<const CmdSetLineCap&>(other)._cap;
  return true;
}

// ===== CmdSetLineJoin =====
void CmdSetLineJoin::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetLineJoin::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setLineJoin(_join);
}

nlohmann::json CmdSetLineJoin::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"join", static_cast<int>(_join)}};
}

bool CmdSetLineJoin::doMerge(const Command& other) {
  _join = static_cast<const CmdSetLineJoin&>(other)._join;
  return true;
}

// ===== CmdSetMiterLimit =====
void CmdSetMiterLimit::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetMiterLimit::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setMiterLimit(_limit);
}

nlohmann::json CmdSetMiterLimit::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"limit", _limit}};
}

bool CmdSetMiterLimit::doMerge(const Command& other) {
  _limit = static_cast<const CmdSetMiterLimit&>(other)._limit;
  return true;
}

// ===== CmdSetLineWidth =====
void CmdSetLineWidth::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetLineWidth::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setLineWidth(_width);
}

nlohmann::json CmdSetLineWidth::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"width", _width}};
}

bool CmdSetLineWidth::doMerge(const Command& other) {
  _width = static_cast<const CmdSetLineWidth&>(other)._width;
  return true;
}

// ===== CmdSetLineDashPattern =====
void CmdSetLineDashPattern::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetLineDashPattern::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setLineDashPattern(_pattern);
}

nlohmann::json CmdSetLineDashPattern::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"pattern", _pattern}};
}

bool CmdSetLineDashPattern::doMerge(const Command& other) {
  _pattern = static_cast<const CmdSetLineDashPattern&>(other)._pattern;
  return true;
}

// ===== CmdSetLineDashPhase =====
void CmdSetLineDashPhase::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetLineDashPhase::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setLineDashPhase(_phase);
}

nlohmann::json CmdSetLineDashPhase::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"phase", _phase}};
}

bool CmdSetLineDashPhase::doMerge(const Command& other) {
  _phase = static_cast<const CmdSetLineDashPhase&>(other)._phase;
  return true;
}

// ===== CmdSetStrokeStart =====
void CmdSetStrokeStart::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetStrokeStart::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setStrokeStart(_start);
}

nlohmann::json CmdSetStrokeStart::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"start", _start}};
}

bool CmdSetStrokeStart::doMerge(const Command& other) {
  _start = static_cast<const CmdSetStrokeStart&>(other)._start;
  return true;
}

// ===== CmdSetStrokeEnd =====
void CmdSetStrokeEnd::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetStrokeEnd::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setStrokeEnd(_end);
}

nlohmann::json CmdSetStrokeEnd::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"end", _end}};
}

bool CmdSetStrokeEnd::doMerge(const Command& other) {
  _end = static_cast<const CmdSetStrokeEnd&>(other)._end;
  return true;
}

// ===== CmdSetStrokeAlign =====
void CmdSetStrokeAlign::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it == objMap.end()) {
    std::cerr << "异常: CmdSetStrokeAlign::execute 未找到 id = " << _id << std::endl;
    return;
  }
  std::static_pointer_cast<ShapeLayer>(it->second)->setStrokeAlign(_align);
}

nlohmann::json CmdSetStrokeAlign::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"align", static_cast<int>(_align)}};
}

bool CmdSetStrokeAlign::doMerge(const Command& other) {
  _align = static_cast<const CmdSetStrokeAlign&>(other)._align;
  return true;

}

}  // namespace tgfx
