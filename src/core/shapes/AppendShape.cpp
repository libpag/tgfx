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

#include "AppendShape.h"
#include "MergeShape.h"
#include <nlohmann/json.hpp>

namespace tgfx {
Rect AppendShape::getBounds(float resolutionScale) const {
  auto bounds = Rect::MakeEmpty();
  for (const auto& shape : shapes) {
    bounds.join(shape->getBounds(resolutionScale));
  }
  return bounds;
}

Path AppendShape::getPath(float resolutionScale) const {
  Path path = {};
  for (const auto& shape : shapes) {
    path.addPath(shape->getPath(resolutionScale));
  }
  return path;
}

std::string AppendShape::toJson() const {
  nlohmann::json jsonObj;
  jsonObj["type"] = "AppendShape";
  jsonObj["shapes"] = nlohmann::json::array();
  for (const auto& shape : shapes) {
    jsonObj["shapes"].push_back(shape->toJson());
  }
  return jsonObj.dump();
}

std::shared_ptr<Shape> AppendShape::FromJson(const std::string& dump) {
  nlohmann::json jsonObj = nlohmann::json::parse(dump);
  std::vector<std::shared_ptr<Shape>> shapesVec;
  for (const auto& shapeJson : jsonObj["shapes"]) {
    shapesVec.push_back(Shape::FromJson(shapeJson.get<std::string>()));
  }
  return std::make_shared<AppendShape>(shapesVec);
}
}  // namespace tgfx
