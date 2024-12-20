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

bool CmdMakeShapeLayer::doMerge(const Command&) {
  // 正常不可能make同一个对象(相同id、相同type）多次，可能上游出现错误了，打印一下
  std::cerr << "异常: CmdMakeShapeLayer::doMerge, id = " << _id << std::endl;
  // 返回true，不需要重复make
  return true;
}
bool CmdSetPath::doMerge(const Command& other) {
  _path = static_cast<const CmdSetPath&>(other)._path;
  return true;
}
bool CmdSetShape::doMerge(const Command& other) {
  _shape = static_cast<const CmdSetShape&>(other)._shape;
  return true;
}
bool CmdSetFillStyle::doMerge(const Command& other) {
  _styleId = static_cast<const CmdSetFillStyle&>(other)._styleId;
  return true;
}
bool CmdSetStrokeStyle::doMerge(const Command& other) {
  _styleId = static_cast<const CmdSetStrokeStyle&>(other)._styleId;
  return true;
}
bool CmdSetLineCap::doMerge(const Command& other) {
  _cap = static_cast<const CmdSetLineCap&>(other)._cap;
  return true;
}
bool CmdSetLineJoin::doMerge(const Command& other) {
  _join = static_cast<const CmdSetLineJoin&>(other)._join;
  return true;
}
bool CmdSetMiterLimit::doMerge(const Command& other) {
  _limit = static_cast<const CmdSetMiterLimit&>(other)._limit;
  return true;
}
bool CmdSetLineWidth::doMerge(const Command& other) {
  _width = static_cast<const CmdSetLineWidth&>(other)._width;
  return true;
}
bool CmdSetLineDashPattern::doMerge(const Command& other) {
  _pattern = static_cast<const CmdSetLineDashPattern&>(other)._pattern;
  return true;
}
bool CmdSetLineDashPhase::doMerge(const Command& other) {
  _phase = static_cast<const CmdSetLineDashPhase&>(other)._phase;
  return true;
}
bool CmdSetStrokeStart::doMerge(const Command& other) {
  _start = static_cast<const CmdSetStrokeStart&>(other)._start;
  return true;
}
bool CmdSetStrokeEnd::doMerge(const Command& other) {
  _end = static_cast<const CmdSetStrokeEnd&>(other)._end;
  return true;
}
bool CmdSetStrokeAlign::doMerge(const Command& other) {
  _align = static_cast<const CmdSetStrokeAlign&>(other)._align;
  return true;
}

void CmdMakeShapeLayer::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  objMap[_id] = ShapeLayer::Make();
}

nlohmann::json CmdMakeShapeLayer::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}};
}

void CmdSetPath::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setPath(_path);
  }
}

nlohmann::json CmdSetPath::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"path", _path.toJson()}};
}

void CmdSetShape::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setShape(_shape);
  }
}

nlohmann::json CmdSetShape::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"shape", _shape->toJson()}};
}

void CmdSetFillStyle::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  auto styleIt = objMap.find(_styleId);
  if (it != objMap.end() && styleIt != objMap.end()) {
    auto style = std::static_pointer_cast<ShapeStyle>(styleIt->second);
    std::static_pointer_cast<ShapeLayer>(it->second)->setFillStyle(style);
  }
}

nlohmann::json CmdSetFillStyle::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"styleId", _styleId}};
}

void CmdSetStrokeStyle::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  auto styleIt = objMap.find(_styleId);
  if (it != objMap.end()) {
    auto style = std::static_pointer_cast<ShapeStyle>(styleIt->second);
    std::static_pointer_cast<ShapeLayer>(it->second)->setStrokeStyle(style);
  }
}

nlohmann::json CmdSetStrokeStyle::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"styleId", _styleId}};
}

void CmdSetLineCap::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setLineCap(_cap);
  }
}

nlohmann::json CmdSetLineCap::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"cap", static_cast<int>(_cap)}};
}

void CmdSetLineJoin::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setLineJoin(_join);
  }
}

nlohmann::json CmdSetLineJoin::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"join", static_cast<int>(_join)}};
}

void CmdSetMiterLimit::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setMiterLimit(_limit);
  }
}

nlohmann::json CmdSetMiterLimit::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"limit", _limit}};
}

void CmdSetLineWidth::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setLineWidth(_width);
  }
}

nlohmann::json CmdSetLineWidth::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"width", _width}};
}

void CmdSetLineDashPattern::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setLineDashPattern(_pattern);
  }
}

nlohmann::json CmdSetLineDashPattern::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"pattern", _pattern}};
}

void CmdSetLineDashPhase::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setLineDashPhase(_phase);
  }
}

nlohmann::json CmdSetLineDashPhase::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"phase", _phase}};
}

void CmdSetStrokeStart::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setStrokeStart(_start);
  }
}

nlohmann::json CmdSetStrokeStart::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"start", _start}};
}

void CmdSetStrokeEnd::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setStrokeEnd(_end);
  }
}

nlohmann::json CmdSetStrokeEnd::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"end", _end}};
}

void CmdSetStrokeAlign::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::static_pointer_cast<ShapeLayer>(it->second)->setStrokeAlign(_align);
  }
}

nlohmann::json CmdSetStrokeAlign::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}, {"align", static_cast<int>(_align)}};
}

}  // namespace tgfx
