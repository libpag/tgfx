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
#include <tgfx/layers/Layer.h>

namespace tgfx {

void CmdSetDefaultAllowsEdgeAntialiasing::execute(std::map<int, Recordable*> objMap) {
  Layer::SetDefaultAllowsEdgeAntialiasing(_value);
}

void CmdSetDefaultAllowsGroupOpacity::execute(std::map<int, Recordable*> objMap) {
  Layer::SetDefaultAllowsGroupOpacity(_value);
}
void CmdMakeLayer::execute(std::map<int, Recordable*> objMap) {
  auto layer = Layer::Make();
  layer->_uuid = _id;
}
void CmdSetAlpha::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setAlpha(_alpha);
  }
}
void CmdSetBlendMode::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setBlendMode(_blendMode);
  }
}
void CmdSetPosition::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setPosition(_position);
  }
}
void CmdSetMatrix::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setMatrix(_matrix);
  }
}
void CmdSetVisible::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setVisible(_visible);
  }
}
void CmdSetShouldRasterize::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setShouldRasterize(_shouldRasterize);
  }
}
void CmdSetRasterizationScale::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setRasterizationScale(_scale);
  }
}
void CmdSetAllowsEdgeAntialiasing::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setAllowsEdgeAntialiasing(_allows);
  }
}
void CmdSetAllowsGroupOpacity::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setAllowsGroupOpacity(_allows);
  }
}
void CmdSetFilters::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setFilters(_filter_ids);
  }
}
void CmdSetMask::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  auto maskIt = objMap.find(_mask_id);
  if (it != objMap.end() && maskIt != objMap.end()) {
    static_cast<Layer*>(it->second)->setMask(static_cast<Layer*>(maskIt->second));
  }
}
void CmdSetScrollRect::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->setScrollRect(_rect);
  }
}
void CmdAddChildAt::execute(std::map<int, Recordable*> objMap) {
  auto parentIt = objMap.find(_id);
  auto childIt = objMap.find(_child_id);
  if (parentIt != objMap.end() && childIt != objMap.end()) {
    static_cast<Layer*>(parentIt->second)->addChildAt(static_cast<Layer*>(childIt->second), _index);
  }
}
void CmdRemoveChildAt::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->removeChildAt(_index);
  }
}
void CmdRemoveChildren::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->removeChildren(_beginIndex, _endIndex);
  }
}
void CmdRemoveFromParent::execute(std::map<int, Recordable*> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second)->removeFromParent();
  }
}
void CmdSetChildIndex::execute(std::map<int, Recordable*> objMap) {
  auto parentIt = objMap.find(_id);
  auto childIt = objMap.find(_child_id);
  if (parentIt != objMap.end() && childIt != objMap.end()) {
    static_cast<Layer*>(parentIt->second)->setChildIndex(static_cast<Layer*>(childIt->second), _index);
  }
}
void CmdReplaceChild::execute(std::map<int, Recordable*> objMap) {
  auto parentIt = objMap.find(_id);
  auto oldChildIt = objMap.find(_oldChild_id);
  auto newChildIt = objMap.find(_newChild_id);
  if (parentIt != objMap.end() && oldChildIt != objMap.end() && newChildIt != objMap.end()) {
    static_cast<Layer*>(parentIt->second)->replaceChild(static_cast<Layer*>(oldChildIt->second), static_cast<Layer*>(newChildIt->second));
  }
}
}  // namespace tgfx
