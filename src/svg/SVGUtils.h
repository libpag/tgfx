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

#include <string>
#include "tgfx/core/Color.h"
#include "tgfx/core/Data.h"
#include "tgfx/core/Image.h"
#include "tgfx/core/Matrix.h"
#include "tgfx/core/Stroke.h"
#include "tgfx/core/Surface.h"
#include "tgfx/gpu/Context.h"

namespace tgfx {

std::string SVGTransform(const Matrix& matrix);

/*
 * For maximum compatibility, do not convert colors to named colors, convert them to hex strings.
*/
std::string SVGColor(Color color);

std::string SVGCap(LineCap cap);

std::string SVGJoin(LineJoin join);

std::string SVGBlendMode(BlendMode mode);

/*
 * Retain four decimal places and remove trailing zeros.
*/
std::string FloatToString(float value);

void base64Encode(unsigned char const* bytes_to_encode, size_t in_len, char* ret);

/*
 * Returns data uri from bytes.
 * it will use any cached data if available, otherwise will encode as png.
*/
std::shared_ptr<Data> AsDataUri(Context* GPUContext, const std::shared_ptr<Image>& image);

}  // namespace tgfx