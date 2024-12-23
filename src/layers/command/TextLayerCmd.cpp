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

#include "TextLayerCmd.h"
#include <tgfx/layers/TextLayer.h>
#include <iostream>

namespace tgfx {

std::unique_ptr<Command> TextLayerCmdFactory::MakeFrom(const nlohmann::json& json) {

  int type = json.at("type").get<int>();
  int id = json.at("id").get<int>();  // 提取 _id
  if (type == TextLayerCommandType::MakeTextLayer) {
    return std::make_unique<CmdMakeTextLayer>(id);
  }
  return nullptr;
}


void CmdMakeTextLayer::execute(std::map<int, std::shared_ptr<Recordable>>& objMap) {
  objMap[_id] = TextLayer::Make();
}

nlohmann::json CmdMakeTextLayer::toJson() const {
  return {{"type", static_cast<int>(getType())}, {"id", _id}};
}

bool CmdMakeTextLayer::doMerge(const Command& other) {
  // 通常不可能多次创建同一个对象（相同id和type），记录异常
  std::cerr << "异常: CmdMakeSolidLayer::doMerge, id = " << _id << std::endl;
  // 返回true，避免重复创建
  return true;
}

}  // namespace tgfx
