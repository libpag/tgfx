/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Tencent is pleased to support the open source community by making tgfx available.
//
//  Copyright (C) 2023 THL A29 Limited, a Tencent company. All rights reserved.
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

#pragma once

#include "gpu/processors/FragmentProcessor.h"

namespace tgfx {
class ComposeFragmentProcessor : public FragmentProcessor {
 public:
  static std::unique_ptr<FragmentProcessor> Make(std::unique_ptr<FragmentProcessor> first,
                                                 std::unique_ptr<FragmentProcessor> second);

  static std::unique_ptr<FragmentProcessor> Make(
      std::vector<std::unique_ptr<FragmentProcessor>> processors);

  std::string name() const override {
    return "ComposeFragmentProcessor";
  }

 protected:
  DEFINE_PROCESSOR_CLASS_ID

  explicit ComposeFragmentProcessor(std::vector<std::unique_ptr<FragmentProcessor>> processors);
};
}  // namespace tgfx