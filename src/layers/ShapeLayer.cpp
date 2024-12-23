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

#include "tgfx/layers/ShapeLayer.h"
#include <layers/record/ShapeLayerRecorder.h>
#include <iostream>
#include "core/utils/Profiling.h"
#include "layers/contents/ShapeContent.h"
#include "tgfx/core/PathEffect.h"
#include "tgfx/core/PathMeasure.h"

#define ENABLE_METHOD_LOGGING 1
#include "core/utils/Log.h"

namespace tgfx {
std::shared_ptr<ShapeLayer> ShapeLayer::Make() {
  TRACE_EVENT;
  auto layer = std::shared_ptr<ShapeLayer>(new ShapeLayer());
  layer->weakThis = layer;
  ShapeLayerRecorder::MakeShapeLayer(layer.get());
  return layer;
}

Path ShapeLayer::path() const {
  if (_shape == nullptr) {
    return {};
  }
  Path path = {};
  if (_shape->isSimplePath(&path)) {
    return path;
  }
  return {};
}

void ShapeLayer::setPath(Path path) {
  Path oldPath = {};
  if (_shape && _shape->isSimplePath(&oldPath) && oldPath == path) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setPath(this, path);
  _shape = Shape::MakeFrom(std::move(path));
  invalidateContent();
}

void ShapeLayer::setShape(std::shared_ptr<Shape> value) {
  if (_shape == value) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setShape(this, value);
  _shape = std::move(value);
  invalidateContent();
}

void ShapeLayer::setFillStyle(std::shared_ptr<ShapeStyle> style) {
  if (_fillStyle == style) {
    return;
  }
  LOG_METHOD("style is " + style->toDebugString());
  ShapeLayerRecorder::setFillStyle(this, style);
  detachProperty(_fillStyle.get());
  _fillStyle = std::move(style);
  attachProperty(_fillStyle.get());
  invalidateContent();
}

void ShapeLayer::setStrokeStyle(std::shared_ptr<ShapeStyle> style) {
  if (_strokeStyle == style) {
    return;
  }
  LOG_METHOD("style is " + style->toDebugString());
  ShapeLayerRecorder::setStrokeStyle(this, style);
  detachProperty(_strokeStyle.get());
  _strokeStyle = std::move(style);
  attachProperty(_strokeStyle.get());
  invalidateContent();
}

void ShapeLayer::setLineCap(LineCap cap) {
  if (stroke.cap == cap) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setLineCap(this, cap);
  stroke.cap = cap;
  invalidateContent();
}

void ShapeLayer::setLineJoin(LineJoin join) {
  if (stroke.join == join) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setLineJoin(this, join);
  stroke.join = join;
  invalidateContent();
}

void ShapeLayer::setMiterLimit(float limit) {
  if (stroke.miterLimit == limit) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setMiterLimit(this, limit);
  stroke.miterLimit = limit;
  invalidateContent();
}

void ShapeLayer::setLineWidth(float width) {
  if (stroke.width == width) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setLineWidth(this, width);
  stroke.width = width;
  invalidateContent();
}

void ShapeLayer::setLineDashPattern(const std::vector<float>& pattern) {
  if (_lineDashPattern.size() == pattern.size() &&
      std::equal(_lineDashPattern.begin(), _lineDashPattern.end(), pattern.begin())) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setLineDashPattern(this, pattern);
  _lineDashPattern = pattern;
  invalidateContent();
}

void ShapeLayer::setLineDashPhase(float phase) {
  if (_lineDashPhase == phase) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setLineDashPhase(this, phase);
  _lineDashPhase = phase;
  invalidateContent();
}

void ShapeLayer::setStrokeStart(float start) {
  if (start < 0) {
    start = 0;
  }
  if (start > 1.0f) {
    start = 1.0f;
  }
  if (_strokeStart == start) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setStrokeStart(this, start);
  _strokeStart = start;
  invalidateContent();
}

void ShapeLayer::setStrokeEnd(float end) {
  if (end < 0) {
    end = 0;
  }
  if (end > 1.0f) {
    end = 1.0f;
  }
  if (_strokeEnd == end) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setStrokeEnd(this, end);
  _strokeEnd = end;
  invalidateContent();
}

void ShapeLayer::setStrokeAlign(StrokeAlign align) {
  if (_strokeAlign == align) {
    return;
  }
  LOG_METHOD("");
  ShapeLayerRecorder::setStrokeAlign(this, align);
  _strokeAlign = align;
  invalidateContent();
}

ShapeLayer::~ShapeLayer() {
  detachProperty(_strokeStyle.get());
  detachProperty(_fillStyle.get());
}

std::unique_ptr<LayerContent> ShapeLayer::onUpdateContent() {
  std::vector<std::unique_ptr<LayerContent>> contents = {};
  if (_shape == nullptr) {
    return nullptr;
  }
  if (_fillStyle) {
    auto content = std::make_unique<ShapeContent>(_shape, _fillStyle->getShader());
    contents.push_back(std::move(content));
  }
  if (stroke.width > 0 && _strokeStyle) {
    auto strokeShape = _shape;
    if ((_strokeStart != 0 || _strokeEnd != 1)) {
      auto pathEffect = PathEffect::MakeTrim(_strokeStart, _strokeEnd);
      strokeShape = Shape::ApplyEffect(std::move(strokeShape), std::move(pathEffect));
    }
    if (!_lineDashPattern.empty()) {
      auto dashes = _lineDashPattern;
      if (_lineDashPattern.size() % 2 != 0) {
        dashes.insert(dashes.end(), _lineDashPattern.begin(), _lineDashPattern.end());
      }
      auto pathEffect =
          PathEffect::MakeDash(dashes.data(), static_cast<int>(dashes.size()), _lineDashPhase);
      strokeShape = Shape::ApplyEffect(std::move(strokeShape), std::move(pathEffect));
    }
    if (_strokeAlign != StrokeAlign::Center) {
      auto tempStroke = stroke;
      tempStroke.width *= 2;
      strokeShape = Shape::ApplyStroke(std::move(strokeShape), &tempStroke);
      if (_strokeAlign == StrokeAlign::Inside) {
        strokeShape = Shape::Merge(std::move(strokeShape), _shape, PathOp::Intersect);
      } else {
        strokeShape = Shape::Merge(std::move(strokeShape), _shape, PathOp::Difference);
      }
    } else {
      strokeShape = Shape::ApplyStroke(std::move(strokeShape), &stroke);
    }
    auto content =
        std::make_unique<ShapeContent>(std::move(strokeShape), _strokeStyle->getShader());
    contents.push_back(std::move(content));
  }
  return LayerContent::Compose(std::move(contents));
}
}  // namespace tgfx
