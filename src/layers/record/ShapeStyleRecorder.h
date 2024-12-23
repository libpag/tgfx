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
struct Point;
class ShapeStyle;

/**
 * ShapeStyleRecorder 负责记录 ShapeStyle 的操作。
 */
class ShapeStyleRecorder {
 public:

  // ----------------- SolidColor -----------------
    /**
     * 录制创建 SolidColor
     */
    static void MakeSolidColor(ShapeStyle* style, const Color& color);

        /**
     * 录制设置颜色
     */
    static void setColor(ShapeStyle* style, const Color& color);


  // ----------------- Gradient -----------------

    /**
     * 录制创建 LinearGradient
     */
    static void MakeLinear(ShapeStyle* style, const Point& startPoint, const Point& endPoint);

    /**
     * 录制创建 RadialGradient
     */
    static void MakeRadial(ShapeStyle* style, const Point& center, float radius);

    /**
     * 录制创建 ConicGradient
     */
    static void MakeConic(ShapeStyle* style, const Point& center, float startAngle, float endAngle);

    /**
     * 录制设置颜色
     */
    static void setColors(ShapeStyle* style, const std::vector<Color>& colors);

    /**
     * 录制设置位置
     */
    static void setPositions(ShapeStyle* style, const std::vector<float>& positions);

    /**
     * 录制设置终点
     */
    static void setEndPoint(ShapeStyle* style, const Point& endPoint);

    /**
     * 录制设置起点
     */
    static void setStartPoint(ShapeStyle* style, const Point& startPoint);

    /**
     * 录制设置中心
     */
    static void setCenter(ShapeStyle* style, const Point& center);

    /**
     * 录制设置半径
     */
    static void setRadius(ShapeStyle* style, float radius);

    /**
     * 录制设置起始角度
     */
    static void setStartAngle(ShapeStyle* style, float startAngle);

    /**
     * 录制设置结束角度
     */
    static void setEndAngle(ShapeStyle* style, float endAngle);
};

}  // namespace tgfx