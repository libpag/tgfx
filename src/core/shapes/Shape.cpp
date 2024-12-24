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

#include "tgfx/core/Shape.h"
#include "core/GlyphRunList.h"
#include "core/shapes/AppendShape.h"
#include "core/shapes/EffectShape.h"
#include "core/shapes/GlyphShape.h"
#include "core/shapes/MatrixShape.h"
#include "core/shapes/MergeShape.h"
#include "core/shapes/PathShape.h"
#include "core/shapes/StrokeShape.h"
#include "nlohmann/json.hpp"

namespace tgfx {
bool Shape::isLine(Point*) const {
  return false;
}

bool Shape::isRect(Rect*) const {
  return false;
}

bool Shape::isOval(Rect*) const {
  return false;
}

bool Shape::isRRect(RRect*) const {
  return false;
}

bool Shape::isSimplePath(Path*) const {
  return false;
}

bool Shape::isInverseFillType() const {
  return false;
}

Rect Shape::getBounds(float) const {
  return Rect::MakeEmpty();
}

Path Shape::getPath(float) const {
  return {};
}

std::shared_ptr<Shape> Shape::FromJson(const std::string& jsonStr) {
  // 解析JSON字符串
  nlohmann::json json = nlohmann::json::parse(jsonStr);
  Type type = static_cast<Type>(json.at("type").get<int>());
  std::shared_ptr<Shape> shape;
  switch (type) {

    case Type::Append:
      break;
    case Type::Effect:
      break;
    case Type::Glyph:
      break;
    case Type::Matrix:
      break;
    case Type::Merge:
      break;
    case Type::Path: {
      Path path;
      path.fromString(json.at("path").get<std::string>());
      shape = std::make_shared<PathShape>(path);
      break;
    }

    case Type::Stroke: {
      Stroke stroke;
      // 解析 stroke
      if (json.contains("stroke")) {
        stroke.width = json["stroke"].value("width", 1.0f);
        stroke.cap = static_cast<LineCap>(json["stroke"].value("cap", 0));
        stroke.join = static_cast<LineJoin>(json["stroke"].value("join", 0));
        stroke.miterLimit = json["stroke"].value("miterLimit", 4.0f);
      }
      std::shared_ptr<Shape> inShape;
      // 解析 shape
      if (json.contains("shape")) {
        inShape = Shape::FromJson(json["shape"].get<std::string>());
      }
      shape = std::make_shared<StrokeShape>(std::move(inShape), stroke);
      break;
    }
  }
  // 配置对象属性
  shape->configFromJson(json.dump());
  return shape;
}

void Shape::configFromJson(const std::string&) {
  // do nothing
}

std::string Shape::toJson() const {
  nlohmann::json json;
  // 添加类型信息
  json["type"] = type();
  return json.dump();
}
std::string Shape::typeToString() {
  switch (type()) {
    case Type::Append:
      return "Append";
    case Type::Effect:
      return "Effect";
    case Type::Glyph:
      return "Glyph";
    case Type::Matrix:
      return "Matrix";
    case Type::Merge:
      return "Merge";
    case Type::Path:
      return "Path";
    case Type::Stroke:
      return "Stroke";
    default:
      return "Unknown";
  }
}


}  // namespace tgfx