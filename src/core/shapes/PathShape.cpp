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

#include <nlohmann/json.hpp>
#include "PathShape.h"
#include "core/PathRef.h"

namespace tgfx {
Rect PathShape::getBounds(float) const {
  return path.getBounds();
}

Path PathShape::getPath(float) const {
  return path;
}

bool PathShape::isLine(Point line[2]) const {
  return path.isLine(line);
}

bool PathShape::isRect(Rect* rect) const {
  return path.isRect(rect);
}

bool PathShape::isOval(Rect* bounds) const {
  return path.isOval(bounds);
}

bool PathShape::isRRect(RRect* rRect) const {
  return path.isRRect(rRect);
}

bool PathShape::isSimplePath(Path* result) const {
  *result = path;
  return true;
}

UniqueKey PathShape::getUniqueKey() const {
  return PathRef::GetUniqueKey(path);
}

void PathShape::configFromJson(const std::string& jsonStr) {
  // 调用父类的配置函数
  Shape::configFromJson(jsonStr);
  nlohmann::json json = nlohmann::json::parse(jsonStr);
  // 还原PathShape的属性
  if (json.contains("path")) {
    this->path = Path();
    this->path.fromJson(json.at("path"));
  }
}

std::string PathShape::toJson() const {
  // 获取父类的JSON
  nlohmann::json json = nlohmann::json::parse(Shape::toJson());
  // 添加PathShape特有的属性
  json["path"] = this->path.toJson();

  return json.dump();
}

}  // namespace tgfx
