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

#include <tgfx/layers/record/Recorder.h>
#include "layers/command/Commands.h"

namespace tgfx {
std::vector<std::unique_ptr<Command>> Recorder::commands_;

void Recorder::Replay(std::string jsonStr, std::map<int, std::shared_ptr<Recordable>>& objMap) {
  // 解析 jsonStr
  auto json = nlohmann::json::parse(jsonStr);
  // 遍历命令
  for (const auto& cmdJson : json) {
    auto cmd = Command::MakeFrom(cmdJson);
    if (cmd) {
      cmd->execute(objMap);
    }
  }
}

std::string Recorder::FlushCommands() {
  if (commands_.empty()) {
    return "";
  }
  // 将 commands_ 转换为 json
  nlohmann::json jsonArray = nlohmann::json::array();
  for (const auto& cmd : commands_) {
    jsonArray.push_back(cmd->toJson());
  }
  // 清空 commands_
  commands_.clear();
  // 返回 json 字符串
  return jsonArray.dump();
}

void Recorder::Record(std::unique_ptr<Command> command) {
  // 从后往前找，找到可以合并的命令
  for (auto it = commands_.rbegin(); it != commands_.rend(); ++it) {
    // 如果可以合并，结束循环
    if ((*it)->merge(*command)) {
      return;
    }
  }
  // 不能合并再添加到 commands_ 中
  commands_.push_back(std::move(command));
}

void Recorder::Remove(int uuid) {
#ifdef ENABLE_REMOVE_COMMAND
  // 从 commands_ 中移除 uuid 对应的命令
  commands_.erase(std::remove_if(commands_.begin(), commands_.end(),
                                 [uuid](const auto& cmd) { return cmd->_id == uuid;
  }), commands_.end());
#endif
}


}  // namespace tgfx
