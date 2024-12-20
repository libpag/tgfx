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

#include "Commands.h"
#include <iostream>
#include "LayerCmd.h"
#include "ShapeLayerCmd.h"
#include "SolidLayerCmd.h"
#include "tgfx/core/Color.h"

namespace tgfx {

nlohmann::json Command::ColorToJson(const Color& color) {
  return {
    {"red", color.red},
    {"green", color.green},
    {"blue", color.blue},
    {"alpha", color.alpha}
  };
}

Color Command::JsonToColor(const nlohmann::json& json) {
  return Color{
    json.value("red", 0.0f),
    json.value("green", 0.0f),
    json.value("blue", 0.0f),
    json.value("alpha", 1.0f)
  };
}

std::unique_ptr<Command> Command::MakeFrom(const nlohmann::json& json) {
  CommandType type = static_cast<CommandType>(json.at("type").get<int>());
  int id = json.at("id").get<int>();  // 提取 _id
  switch (type) {
    // ------------------- Layer -------------------
    case CommandType::SetDefaultAllowsEdgeAntialiasing:
      return std::make_unique<CmdSetDefaultAllowsEdgeAntialiasing>(id,
                                                                   json.at("value").get<bool>());
    case CommandType::SetDefaultAllowsGroupOpacity:
      return std::make_unique<CmdSetDefaultAllowsGroupOpacity>(id, json.at("value").get<bool>());
    case CommandType::MakeLayer:
      return std::make_unique<CmdMakeLayer>(id);
    case CommandType::setName: {
      auto name = json.at("name").get<std::string>();
      return std::make_unique<CmdSetName>(id, name);
    }
    case CommandType::setAlpha:
      return std::make_unique<CmdSetAlpha>(id, json.at("alpha").get<float>());
    case CommandType::setBlendMode:
      return std::make_unique<CmdSetBlendMode>(
          id, static_cast<BlendMode>(json.at("blendMode").get<int>()));
    case CommandType::setPosition: {
      auto pos = json.at("position");
      Point position = Point::Make(pos[0].get<float>(), pos[1].get<float>());
      return std::make_unique<CmdSetPosition>(id, position);
    }
    case CommandType::setMatrix: {
      auto mat = json.at("matrix");
      Matrix matrix = Matrix::MakeAll(mat[0].get<float>(),  // scaleX
                                      mat[1].get<float>(),  // skewX
                                      mat[2].get<float>(),  // transX
                                      mat[3].get<float>(),  // skewY
                                      mat[4].get<float>(),  // scaleY
                                      mat[5].get<float>()   // transY
      );
      return std::make_unique<CmdSetMatrix>(id, matrix);
    }
    case CommandType::setVisible:
      return std::make_unique<CmdSetVisible>(id, json.at("visible").get<bool>());
    case CommandType::setShouldRasterize:
      return std::make_unique<CmdSetShouldRasterize>(id, json.at("shouldRasterize").get<bool>());
    case CommandType::setRasterizationScale:
      return std::make_unique<CmdSetRasterizationScale>(id, json.at("scale").get<float>());
    case CommandType::setAllowsEdgeAntialiasing:
      return std::make_unique<CmdSetAllowsEdgeAntialiasing>(id, json.at("allows").get<bool>());
    case CommandType::setAllowsGroupOpacity:
      return std::make_unique<CmdSetAllowsGroupOpacity>(id, json.at("allows").get<bool>());
    case CommandType::setFilters:
      return std::make_unique<CmdSetFilters>(id, json.at("filter_ids").get<std::vector<int>>());
    case CommandType::setMask:
      return std::make_unique<CmdSetMask>(id, json.at("mask_id").get<int>());
    case CommandType::setScrollRect: {
      auto rect = json.at("rect");
      Rect r = Rect::MakeXYWH(rect[0].get<float>(), rect[1].get<float>(), rect[2].get<float>(),
                              rect[3].get<float>());
      return std::make_unique<CmdSetScrollRect>(id, r);
    }
    case CommandType::addChildAt:
      return std::make_unique<CmdAddChildAt>(id, json.at("child_id").get<int>(),
                                             json.at("index").get<int>());
    case CommandType::removeChildAt:
      return std::make_unique<CmdRemoveChildAt>(id, json.at("index").get<int>());
    case CommandType::removeChildren:
      return std::make_unique<CmdRemoveChildren>(id, json.at("beginIndex").get<int>(),
                                                 json.at("endIndex").get<int>());
    case CommandType::removeFromParent:
      return std::make_unique<CmdRemoveFromParent>(id);
    case CommandType::setChildIndex:
      return std::make_unique<CmdSetChildIndex>(id, json.at("child_id").get<int>(),
                                                json.at("index").get<int>());
    case CommandType::replaceChild:
      return std::make_unique<CmdReplaceChild>(id, json.at("oldChild_id").get<int>(),
                                               json.at("newChild_id").get<int>());
    // ------------------- ShapeLayer -------------------
    case CommandType::MakeShapeLayer:
      return std::make_unique<CmdMakeShapeLayer>(id);
    case CommandType::setPath: {
      Path path;
      path.fromJson(json.at("path").dump());
      return std::make_unique<CmdSetPath>(id, path);
    }
    case CommandType::setShape: {
      auto shapeJson = json.at("shape");
      std::shared_ptr<Shape> shape = Shape::FromJson(shapeJson.dump());
      return std::make_unique<CmdSetShape>(id, shape);
    }
    case CommandType::setFillStyle:
      return std::make_unique<CmdSetFillStyle>(id, json.at("styleId").get<int>());
    case CommandType::setStrokeStyle:
      return std::make_unique<CmdSetStrokeStyle>(id, json.at("styleId").get<int>());
    case CommandType::setLineCap:
      return std::make_unique<CmdSetLineCap>(id, static_cast<LineCap>(json.at("cap").get<int>()));
    case CommandType::setLineJoin:
      return std::make_unique<CmdSetLineJoin>(id,
                                              static_cast<LineJoin>(json.at("join").get<int>()));
    case CommandType::setMiterLimit:
      return std::make_unique<CmdSetMiterLimit>(id, json.at("limit").get<float>());
    case CommandType::setLineWidth:
      return std::make_unique<CmdSetLineWidth>(id, json.at("width").get<float>());
    case CommandType::setLineDashPattern:
      return std::make_unique<CmdSetLineDashPattern>(id,
                                                     json.at("pattern").get<std::vector<float>>());
    case CommandType::setLineDashPhase:
      return std::make_unique<CmdSetLineDashPhase>(id, json.at("phase").get<float>());
    case CommandType::setStrokeStart:
      return std::make_unique<CmdSetStrokeStart>(id, json.at("start").get<float>());
    case CommandType::setStrokeEnd:
      return std::make_unique<CmdSetStrokeEnd>(id, json.at("end").get<float>());
    case CommandType::setStrokeAlign:
      return std::make_unique<CmdSetStrokeAlign>(
          id, static_cast<StrokeAlign>(json.at("align").get<int>()));
    // ------------------- SolidLayerRecorder -------------------
    case CommandType::MakeSolidLayer:
      return std::make_unique<CmdMakeSolidLayer>(id);
    case CommandType::setWidth:
      return std::make_unique<CmdSetWidth>(id, json.at("width").get<float>());
    case CommandType::setHeight:
      return std::make_unique<CmdSetHeight>(id, json.at("height").get<float>());
    case CommandType::setRadiusX:
      return std::make_unique<CmdSetRadiusX>(id, json.at("radiusX").get<float>());
    case CommandType::setRadiusY:
      return std::make_unique<CmdSetRadiusY>(id, json.at("radiusY").get<float>());
    case CommandType::setColor:
      return std::make_unique<CmdSetColor>(id, Command::JsonToColor(json.at("color")));
    // TODO 补齐其他类型
    default:
      throw std::invalid_argument("Unknown CommandType");
  }
}
bool Command::merge(const Command& other) {
  // id 不同，或者 type 不同，不可合并
  if (other._id != _id || other.getType() != getType()) {
    return false;
  }
  return doMerge(other);
}

bool Command::doMerge(const Command& ){
  // 默认不可合并，看子类
  return false;
}

}  // namespace tgfx
