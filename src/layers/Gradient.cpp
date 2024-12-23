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

#include "tgfx/layers/Gradient.h"
#include "record/ShapeStyleRecorder.h"
#include "tgfx/layers/Layer.h"

namespace tgfx {
std::shared_ptr<LinearGradient> Gradient::MakeLinear(const Point& startPoint,
                                                     const Point& endPoint) {
  auto linearGradient = std::shared_ptr<LinearGradient>(new LinearGradient(startPoint, endPoint));
  ShapeStyleRecorder::MakeLinear(linearGradient.get(), startPoint, endPoint);
  return linearGradient;
}

std::shared_ptr<RadialGradient> Gradient::MakeRadial(const Point& center, float radius) {
  auto radialGradient = std::shared_ptr<RadialGradient>(new RadialGradient(center, radius));
  ShapeStyleRecorder::MakeRadial(radialGradient.get(), center, radius);
  return radialGradient;
}

std::shared_ptr<ConicGradient> Gradient::MakeConic(const Point& center, float startAngle,
                                                   float endAngle) {
  auto conicGradient = std::shared_ptr<ConicGradient>(new ConicGradient(center, startAngle, endAngle));
  ShapeStyleRecorder::MakeConic(conicGradient.get(), center, startAngle, endAngle);
  return conicGradient;
}

void Gradient::setColors(std::vector<Color> colors) {
  _colors = std::move(colors);
  invalidate();
  ShapeStyleRecorder::setColors(this, colors);
}

void Gradient::setPositions(std::vector<float> positions) {
  _positions = std::move(positions);
  invalidate();
  ShapeStyleRecorder::setPositions(this, positions);
}

void LinearGradient::setEndPoint(const Point& endPoint) {
  _endPoint = endPoint;
  invalidate();
  ShapeStyleRecorder::setEndPoint(this, endPoint);
}

void LinearGradient::setStartPoint(const Point& startPoint) {
  _startPoint = startPoint;
  invalidate();
  ShapeStyleRecorder::setStartPoint(this, startPoint);
}

std::shared_ptr<Shader> LinearGradient::getShader() const {
  return Shader::MakeLinearGradient(_startPoint, _endPoint, _colors, _positions);
}

void RadialGradient::setCenter(const Point& center) {
  _center = center;
  invalidate();
  ShapeStyleRecorder::setCenter(this, center);
}

void RadialGradient::setRadius(float radius) {
  _radius = radius;
  invalidate();
  ShapeStyleRecorder::setRadius(this, radius);
}

std::shared_ptr<Shader> RadialGradient::getShader() const {
  return Shader::MakeRadialGradient(_center, _radius, _colors, _positions);
}

void ConicGradient::setStartAngle(float startAngle) {
  _startAngle = startAngle;
  invalidate();
  ShapeStyleRecorder::setStartAngle(this, startAngle);
}

void ConicGradient::setCenter(const Point& center) {
  _center = center;
  invalidate();
  ShapeStyleRecorder::setCenter(this, center);
}

void ConicGradient::setEndAngle(float endAngle) {
  _endAngle = endAngle;
  invalidate();
  ShapeStyleRecorder::setEndAngle(this, endAngle);
}

std::shared_ptr<Shader> ConicGradient::getShader() const {
  return Shader::MakeConicGradient(_center, _startAngle, _endAngle, _colors, _positions);
}
}  // namespace tgfx
