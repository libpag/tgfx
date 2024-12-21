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
#pragma once

#include <memory>
#include "tgfx/core/Color.h"

namespace tgfx {

class ShapeStyle;

/**
 * ShapeStyleRecorder 负责记录 ShapeStyle 的操作。
 */
class ShapeStyleRecorder {
 public:
    /**
     * 录制创建 SolidColor
     */
    static void MakeSolidColor(ShapeStyle* style, const Color& color);

    /**
     * 录制设置颜色
     */
    static void setColor(ShapeStyle* style, const Color& color);
};

}  // namespace tgfx