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
#include "tgfx/core/RRect.h"
#include "tgfx/core/Rect.h"
#include "tgfx/svg/SVGTypes.h"
#include "tgfx/svg/node/SVGNode.h"
#include "tgfx/svg/node/SVGShape.h"

namespace tgfx {

class SVGRect final : public SVGShape {
 public:
  static std::shared_ptr<SVGRect> Make() {
    return std::shared_ptr<SVGRect>(new SVGRect());
  }

  SVG_ATTR(X, SVGLength, SVGLength(0))
  SVG_ATTR(Y, SVGLength, SVGLength(0))
  SVG_ATTR(Width, SVGLength, SVGLength(0))
  SVG_ATTR(Height, SVGLength, SVGLength(0))

  SVG_OPTIONAL_ATTR(Rx, SVGLength)
  SVG_OPTIONAL_ATTR(Ry, SVGLength)

 protected:
  bool parseAndSetAttribute(const std::string&, const std::string&) override;

  void onDraw(Canvas*, const SVGLengthContext&, const Paint&, PathFillType) const override;

  Path onAsPath(const SVGRenderContext&) const override;

  Rect onObjectBoundingBox(const SVGRenderContext&) const override;

 private:
  SVGRect();

  RRect resolve(const SVGLengthContext&) const;

  using INHERITED = SVGShape;
};
}  // namespace tgfx