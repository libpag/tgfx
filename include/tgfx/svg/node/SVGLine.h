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
#include "tgfx/core/Canvas.h"
#include "tgfx/core/Paint.h"
#include "tgfx/core/Path.h"
#include "tgfx/core/Point.h"
#include "tgfx/svg/SVGTypes.h"
#include "tgfx/svg/node/SVGNode.h"
#include "tgfx/svg/node/SVGShape.h"

namespace tgfx {

class SVGLine final : public SVGShape {
 public:
  static std::shared_ptr<SVGLine> Make() {
    return std::shared_ptr<SVGLine>(new SVGLine());
  }

  SVG_ATTR(X1, SVGLength, SVGLength(0))
  SVG_ATTR(Y1, SVGLength, SVGLength(0))
  SVG_ATTR(X2, SVGLength, SVGLength(0))
  SVG_ATTR(Y2, SVGLength, SVGLength(0))

 protected:
  bool parseAndSetAttribute(const std::string&, const std::string&) override;

  void onDraw(Canvas*, const SVGLengthContext&, const Paint&, PathFillType) const override;

  Path onAsPath(const SVGRenderContext&) const override;

 private:
  SVGLine();

  // resolve and return the two endpoints
  std::tuple<Point, Point> resolve(const SVGLengthContext&) const;

  using INHERITED = SVGShape;
};

}  // namespace tgfx