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

namespace tgfx {


std::unique_ptr<Command> Command::MakeFrom(const nlohmann::json& json) {
  CommandType type = static_cast<CommandType>(json.at("type").get<int>());
  switch (type) {
    // ------------------- Layer -------------------
    case CommandType::SetDefaultAllowsEdgeAntialiasing:
      return std::make_unique<CmdSetDefaultAllowsEdgeAntialiasing>(json.at("value").get<bool>());
    case CommandType::SetDefaultAllowsGroupOpacity:
      return std::make_unique<CmdSetDefaultAllowsGroupOpacity>(json.at("value").get<bool>());
    case CommandType::MakeLayer:
      return std::make_unique<CmdMakeLayer>(json.at("id").get<int>());
    case CommandType::setName: {
      auto name = json.at("name").get<std::string>();
      std::cout << "json: " << json.dump(4) << "\n\n name  " << name << std::endl;
      return std::make_unique<CmdSetName>(json.at("id").get<int>(), name);
    }
    case CommandType::setAlpha:
      return std::make_unique<CmdSetAlpha>(json.at("id").get<int>(), json.at("alpha").get<float>());
    case CommandType::setBlendMode:
      return std::make_unique<CmdSetBlendMode>(
          json.at("id").get<int>(), static_cast<BlendMode>(json.at("blendMode").get<int>()));
    case CommandType::setPosition: {
      auto pos = json.at("position");
      Point position = Point::Make(pos[0].get<float>(), pos[1].get<float>());
      return std::make_unique<CmdSetPosition>(json.at("id").get<int>(), position);
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
      return std::make_unique<CmdSetMatrix>(json.at("id").get<int>(), matrix);
    }
    case CommandType::setVisible:
      return std::make_unique<CmdSetVisible>(json.at("id").get<int>(),
                                             json.at("visible").get<bool>());
    case CommandType::setShouldRasterize:
      return std::make_unique<CmdSetShouldRasterize>(json.at("id").get<int>(),
                                                     json.at("shouldRasterize").get<bool>());
    case CommandType::setRasterizationScale:
      return std::make_unique<CmdSetRasterizationScale>(json.at("id").get<int>(),
                                                        json.at("scale").get<float>());
    case CommandType::setAllowsEdgeAntialiasing:
      return std::make_unique<CmdSetAllowsEdgeAntialiasing>(json.at("id").get<int>(),
                                                            json.at("allows").get<bool>());
    case CommandType::setAllowsGroupOpacity:
      return std::make_unique<CmdSetAllowsGroupOpacity>(json.at("id").get<int>(),
                                                        json.at("allows").get<bool>());
    case CommandType::setFilters:
      return std::make_unique<CmdSetFilters>(json.at("id").get<int>(),
                                             json.at("filter_ids").get<std::vector<int>>());
    case CommandType::setMask:
      return std::make_unique<CmdSetMask>(json.at("id").get<int>(), json.at("mask_id").get<int>());
    case CommandType::setScrollRect: {
      auto rect = json.at("rect");
      Rect r = Rect::MakeXYWH(rect[0].get<float>(), rect[1].get<float>(), rect[2].get<float>(),
                              rect[3].get<float>());
      return std::make_unique<CmdSetScrollRect>(json.at("id").get<int>(), r);
    }
    case CommandType::addChildAt:
      return std::make_unique<CmdAddChildAt>(
          json.at("id").get<int>(), json.at("child_id").get<int>(), json.at("index").get<int>());
    case CommandType::removeChildAt:
      return std::make_unique<CmdRemoveChildAt>(json.at("id").get<int>(),
                                                json.at("index").get<int>());
    case CommandType::removeChildren:
      return std::make_unique<CmdRemoveChildren>(json.at("id").get<int>(),
                                                 json.at("beginIndex").get<int>(),
                                                 json.at("endIndex").get<int>());
    case CommandType::removeFromParent:
      return std::make_unique<CmdRemoveFromParent>(json.at("id").get<int>());
    case CommandType::setChildIndex:
      return std::make_unique<CmdSetChildIndex>(
          json.at("id").get<int>(), json.at("child_id").get<int>(), json.at("index").get<int>());
    case CommandType::replaceChild:
      return std::make_unique<CmdReplaceChild>(json.at("id").get<int>(),
                                               json.at("oldChild_id").get<int>(),
                                               json.at("newChild_id").get<int>());
    // ------------------- ShapeLayer -------------------
    case CommandType::MakeShapeLayer:
      return std::make_unique<CmdMakeShapeLayer>(json.at("id").get<int>());
    case CommandType::setPath: {
      Path path;
      path.fromJson(json.at("path").dump());
      return std::make_unique<CmdSetPath>(json.at("id").get<int>(), path);
    }
    case CommandType::setShape: {
      auto shapeJson = json.at("shape");
      std::shared_ptr<Shape> shape = Shape::FromJson(shapeJson.dump());
      return std::make_unique<CmdSetShape>(json.at("id").get<int>(), shape);
    }
    case CommandType::setFillStyle:
      return std::make_unique<CmdSetFillStyle>(json.at("id").get<int>(), json.at("styleId").get<int>());
    case CommandType::setStrokeStyle:
      return std::make_unique<CmdSetStrokeStyle>(json.at("id").get<int>(), json.at("styleId").get<int>());
    case CommandType::setLineCap:
      return std::make_unique<CmdSetLineCap>(json.at("id").get<int>(), static_cast<LineCap>(json.at("cap").get<int>()));
    case CommandType::setLineJoin:
      return std::make_unique<CmdSetLineJoin>(json.at("id").get<int>(), static_cast<LineJoin>(json.at("join").get<int>()));
    case CommandType::setMiterLimit:
      return std::make_unique<CmdSetMiterLimit>(json.at("id").get<int>(), json.at("limit").get<float>());
    case CommandType::setLineWidth:
      return std::make_unique<CmdSetLineWidth>(json.at("id").get<int>(), json.at("width").get<float>());
    case CommandType::setLineDashPattern:
      return std::make_unique<CmdSetLineDashPattern>(json.at("id").get<int>(), json.at("pattern").get<std::vector<float>>());
    case CommandType::setLineDashPhase:
      return std::make_unique<CmdSetLineDashPhase>(json.at("id").get<int>(), json.at("phase").get<float>());
    case CommandType::setStrokeStart:
      return std::make_unique<CmdSetStrokeStart>(json.at("id").get<int>(), json.at("start").get<float>());
    case CommandType::setStrokeEnd:
      return std::make_unique<CmdSetStrokeEnd>(json.at("id").get<int>(), json.at("end").get<float>());
    case CommandType::setStrokeAlign:
      return std::make_unique<CmdSetStrokeAlign>(json.at("id").get<int>(), static_cast<StrokeAlign>(json.at("align").get<int>()));
    default:
      throw std::invalid_argument("Unknown CommandType");
  }
}

}  // namespace tgfx
