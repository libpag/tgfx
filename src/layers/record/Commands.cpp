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

void CmdSetDefaultAllowsEdgeAntialiasing::execute(std::map<int, std::shared_ptr<Recordable>> ) {
  Layer::SetDefaultAllowsEdgeAntialiasing(_value);
}

void CmdSetDefaultAllowsGroupOpacity::execute(std::map<int, std::shared_ptr<Recordable>> ) {
  Layer::SetDefaultAllowsGroupOpacity(_value);
}
void CmdMakeLayer::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto layer = Layer::Make();
  layer->_uuid = _id;
  objMap.insert(std::make_pair(_id, layer));
}
void CmdSetAlpha::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setAlpha(_alpha);
  }
}
void CmdSetBlendMode::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setBlendMode(_blendMode);
  }
}
void CmdSetPosition::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setPosition(_position);
  }
}
void CmdSetMatrix::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setMatrix(_matrix);
  }
}
void CmdSetVisible::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setVisible(_visible);
  }
}
void CmdSetShouldRasterize::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setShouldRasterize(_shouldRasterize);
  }
}
void CmdSetRasterizationScale::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setRasterizationScale(_scale);
  }
}
void CmdSetAllowsEdgeAntialiasing::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setAllowsEdgeAntialiasing(_allows);
  }
}
void CmdSetAllowsGroupOpacity::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setAllowsGroupOpacity(_allows);
  }
}
void CmdSetFilters::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    std::vector<std::shared_ptr<LayerFilter>> filters;
    for (int filter_id : _filter_ids) {
      auto filterIt = objMap.find(filter_id);
      if (filterIt != objMap.end()) {
        filters.push_back(std::static_pointer_cast<LayerFilter>(filterIt->second));
      }
    }
    static_cast<Layer*>(it->second.get())->setFilters(filters);
  }
}

void CmdSetMask::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  auto maskIt = objMap.find(_mask_id);
  if (it != objMap.end() && maskIt != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setMask(std::static_pointer_cast<Layer>(maskIt->second));
  }
}
void CmdSetScrollRect::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->setScrollRect(_rect);
  }
}
void CmdAddChildAt::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto parentIt = objMap.find(_id);
  auto childIt = objMap.find(_child_id);
  if (parentIt != objMap.end() && childIt != objMap.end()) {
    static_cast<Layer*>(parentIt->second.get())->addChildAt(std::static_pointer_cast<Layer>(childIt->second), _index);
  }
}
void CmdRemoveChildAt::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->removeChildAt(_index);
  }
}
void CmdRemoveChildren::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->removeChildren(_beginIndex, _endIndex);
  }
}
void CmdRemoveFromParent::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto it = objMap.find(_id);
  if (it != objMap.end()) {
    static_cast<Layer*>(it->second.get())->removeFromParent();
  }
}
void CmdSetChildIndex::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto parentIt = objMap.find(_id);
  auto childIt = objMap.find(_child_id);
  if (parentIt != objMap.end() && childIt != objMap.end()) {

    static_cast<Layer*>(parentIt->second.get())->setChildIndex(std::static_pointer_cast<Layer>(childIt->second), _index);
  }
}
void CmdReplaceChild::execute(std::map<int, std::shared_ptr<Recordable>> objMap) {
  auto parentIt = objMap.find(_id);
  auto oldChildIt = objMap.find(_oldChild_id);
  auto newChildIt = objMap.find(_newChild_id);
  if (parentIt != objMap.end() && oldChildIt != objMap.end() && newChildIt != objMap.end()) {
    static_cast<Layer*>(parentIt->second.get())->replaceChild(std::static_pointer_cast<Layer>(oldChildIt->second),std::static_pointer_cast<Layer>(newChildIt->second));
  }
}
}  // namespace tgfx
