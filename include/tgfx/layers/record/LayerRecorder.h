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

namespace tgfx {
struct Rect;
class LayerFilter;
class Matrix;
struct Point;
enum class BlendMode;
class Layer;
class LayerRecorder {
 public:
 private:
  static void SetDefaultAllowsEdgeAntialiasing(bool value);
  static void SetDefaultAllowsGroupOpacity(bool value);
  static void MakeLayer(Layer* layer);
  static void setName(Layer* layer, const std::string& value);
  static void setAlpha(Layer* layer, float value);
  static void setBlendMode(Layer* layer, BlendMode value);
  static void setPosition(Layer* layer, const Point& value);
  static void setMatrix(Layer* layer, const Matrix& value);
  static void setVisible(Layer* layer, bool value);
  static void setShouldRasterize(Layer* layer, bool value);
  static void setRasterizationScale(Layer* layer, float value);
  static void setAllowsEdgeAntialiasing(Layer* layer, bool value);
  static void setAllowsGroupOpacity(Layer* layer, bool value);
  static void setFilters(Layer* layer,
                         const std::vector<std::shared_ptr<LayerFilter>>& layer_filters);
  static void setMask(Layer* layer, const std::shared_ptr<Layer>& mask);
  static void setScrollRect(Layer* layer, const Rect& rect);
  static void addChildAt(Layer* layer, const std::shared_ptr<Layer>& child, int index);
  static void removeChildAt(Layer* layer, int index);
  static void removeChildren(Layer* layer, int beginIndex, int endIndex);
  static void removeFromParent(Layer* layer);
  static void setChildIndex(Layer* layer, const std::shared_ptr<Layer>& child, int index);
  static void replaceChild(Layer* layer, const std::shared_ptr<Layer>& old_child,
                           const std::shared_ptr<Layer>& new_child);
  static void Remove(int uuid);

  friend class Layer;
};
}  // namespace tgfx
