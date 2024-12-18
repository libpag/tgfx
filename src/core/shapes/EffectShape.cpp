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

#include "EffectShape.h"
#include <nlohmann/json.hpp>

namespace tgfx {
Rect EffectShape::getBounds(float resolutionScale) const {
  auto bounds = shape->getBounds(resolutionScale);
  return effect->filterBounds(bounds);
}

Path EffectShape::getPath(float resolutionScale) const {
  auto path = shape->getPath(resolutionScale);
  effect->filterPath(&path);
  return path;
}

std::string EffectShape::toJson() const {
  nlohmann::json jsonObj;
  jsonObj["type"] = "EffectShape";
  jsonObj["shape"] = shape->toJson();
  // jsonObj["effect"] = effect->toJson();
  return jsonObj.dump();
}

std::shared_ptr<Shape> EffectShape::FromJson(const std::string& dump) {
  nlohmann::json jsonObj = nlohmann::json::parse(dump);
  auto shape = Shape::FromJson(jsonObj["shape"].get<std::string>());
  // auto effect = PathEffect::FromJson(jsonObj["effect"].get<std::string>());
  // return std::make_shared<EffectShape>(shape, effect);
  return std::make_shared<EffectShape>(shape, nullptr);
}

}  // namespace tgfx
