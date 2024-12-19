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

#include <gtest/gtest.h>
#include <math.h>
#include <tgfx/layers/record/Recorder.h>
#include <vector>
#include "core/filters/BlurImageFilter.h"
#include "core/shapes/AppendShape.h"
#include "core/shapes/EffectShape.h"
#include "core/shapes/GlyphShape.h"
#include "core/shapes/MatrixShape.h"
#include "core/shapes/MergeShape.h"
#include "core/shapes/PathShape.h"
#include "core/shapes/StrokeShape.h"
#include "core/utils/Profiling.h"
#include "tgfx/core/Path.h"
#include "tgfx/core/PathEffect.h"
#include "tgfx/layers/DisplayList.h"
#include "tgfx/layers/Gradient.h"
#include "tgfx/layers/ImageLayer.h"
#include "tgfx/layers/Layer.h"
#include "tgfx/layers/ShapeLayer.h"
#include "tgfx/layers/SolidLayer.h"
#include "tgfx/layers/TextLayer.h"
#include "tgfx/layers/filters/BlendFilter.h"
#include "tgfx/layers/filters/BlurFilter.h"
#include "tgfx/layers/filters/ColorMatrixFilter.h"
#include "tgfx/layers/filters/DropShadowFilter.h"
#include "tgfx/layers/filters/InnerShadowFilter.h"
#include "utils/TestUtils.h"
#include "utils/common.h"

namespace tgfx {

// 添加 PathShape 的单元测试
TGFX_TEST(ShapeJsonTest, PathShapeJson) {

  auto device = GLDevice::Make();
  auto context = device->lockContext();
  auto surface = Surface::Make(context, 400, 400);
  auto canvas = surface->getCanvas();

  Paint paint;
  paint.setColor(Color{1.f, 0.f, 0.f, 1.f});
  paint.setStyle(PaintStyle::Fill);
  // -----------------------------------------------------

  // 创建 PathShape 并测试序列化
  Path path;
  path.addRect(Rect::MakeLTRB(0.0f, 0.0f, 100.0f, 100.0f));
  auto pathShape = std::make_shared<PathShape>(std::move(path));
  // -----------------------------------------------------

  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(pathShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/PathShapeJson1"));

  // -----------------------------------------------------
  std::string pathJson = pathShape->toJson();
  auto parsedPathShape = Shape::FromJson(pathJson);
  ASSERT_NE(parsedPathShape, nullptr);
  ASSERT_EQ(parsedPathShape->type(), Shape::Type::Path);

  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(parsedPathShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/PathShapeJson2"));

  device->unlock();
}

// 添加 StrokeShape 的单元测试
TGFX_TEST(ShapeJsonTest, StrokeShapeJson) {
  // 创建 StrokeShape 并测试序列化
  Path path2;
  auto baseShape = std::make_shared<PathShape>(path2);
  Stroke stroke;
  stroke.width = 5.0f;
  auto strokeShape = std::make_shared<StrokeShape>(baseShape, stroke);

  // 添加 Baseline::Compare 前置逻辑
  auto device = GLDevice::Make();
  auto context = device->lockContext();
  auto surface = Surface::Make(context, 400, 400);
  auto canvas = surface->getCanvas();

  Paint paint;
  paint.setColor(Color{1.f, 0.f, 0.f, 1.f});
  paint.setStyle(PaintStyle::Fill);

  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(strokeShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/StrokeShapeJson1"));

  std::string strokeJson = strokeShape->toJson();
  auto parsedStrokeShape = Shape::FromJson(strokeJson);
  ASSERT_NE(parsedStrokeShape, nullptr);
  ASSERT_EQ(parsedStrokeShape->type(), Shape::Type::Stroke);
  // 验证 Stroke 属性
  auto parsedStroke = std::static_pointer_cast<StrokeShape>(parsedStrokeShape)->stroke;
  ASSERT_EQ(stroke.width, parsedStroke.width);
  // 验证 内部 Shape
  ASSERT_EQ(std::static_pointer_cast<StrokeShape>(parsedStrokeShape)->shape->type(),
            Shape::Type::Path);

  // 添加反序列化后的绘制逻辑
  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(parsedStrokeShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/StrokeShapeJson2"));

  device->unlock();
}

// 添加 MergeShape 的单元测试
TGFX_TEST(ShapeJsonTest, MergeShapeJson) {
  // 创建 MergeShape 并测试序列化
  Path path3;
  auto baseShape = std::make_shared<PathShape>(path3);
  auto secondShape = std::make_shared<PathShape>(path3);
  auto mergeShape = std::make_shared<MergeShape>(baseShape, secondShape, PathOp::Union);
  // 添加 Baseline::Compare 前置逻辑
  auto device = GLDevice::Make();
  auto context = device->lockContext();
  auto surface = Surface::Make(context, 400, 400);
  auto canvas = surface->getCanvas();

  Paint paint;
  paint.setColor(Color{1.f, 0.f, 0.f, 1.f});
  paint.setStyle(PaintStyle::Fill);

  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(mergeShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/MergeShapeJson1"));

  std::string mergeJson = mergeShape->toJson();
  auto parsedMergeShape = Shape::FromJson(mergeJson);
  ASSERT_NE(parsedMergeShape, nullptr);
  ASSERT_EQ(parsedMergeShape->type(), Shape::Type::Merge);
  // 验证 Merge 属性
  auto parsedMerge = std::static_pointer_cast<MergeShape>(parsedMergeShape);
  ASSERT_EQ(parsedMerge->pathOp, PathOp::Union);
  ASSERT_EQ(parsedMerge->first->type(), Shape::Type::Path);
  ASSERT_EQ(parsedMerge->second->type(), Shape::Type::Path);

  // 添加反序列化后的绘制逻辑
  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(parsedMergeShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/MergeShapeJson2"));

  device->unlock();
}

// 添加 MatrixShape 的单元测试
TGFX_TEST(ShapeJsonTest, MatrixShapeJson) {
  // 创建 MatrixShape 并测试序列化
  Path path2;
  auto baseShape = std::make_shared<PathShape>(path2);
  Matrix matrix = Matrix::MakeScale(2.0f, 2.0f);
  auto matrixShape = std::make_shared<MatrixShape>(baseShape, matrix);
  // 添加 Baseline::Compare 前置逻辑
  auto device = GLDevice::Make();
  auto context = device->lockContext();
  auto surface = Surface::Make(context, 400, 400);
  auto canvas = surface->getCanvas();

  Paint paint;
  paint.setColor(Color{1.f, 0.f, 0.f, 1.f});
  paint.setStyle(PaintStyle::Fill);

  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(matrixShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/MatrixShapeJson1"));

  std::string matrixJson = matrixShape->toJson();
  auto parsedMatrixShape = Shape::FromJson(matrixJson);
  ASSERT_NE(parsedMatrixShape, nullptr);
  ASSERT_EQ(parsedMatrixShape->type(), Shape::Type::Matrix);
  // 验证 Matrix 属性
  auto parsedMatrix = std::static_pointer_cast<MatrixShape>(parsedMatrixShape);
  ASSERT_EQ(parsedMatrix->matrix, matrix);
  ASSERT_EQ(parsedMatrix->shape->type(), Shape::Type::Path);

  // 添加反序列化后的绘制逻辑
  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(parsedMatrixShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/MatrixShapeJson2"));

  device->unlock();
}

// 添加 GlyphShape 的单元测试
TGFX_TEST(ShapeJsonTest, GlyphShapeJson) {
  // 创建 GlyphShape 并测试序列化
  GlyphRun glyphRun;
  auto glyphRunList = std::make_shared<GlyphRunList>(glyphRun);
  // 假设 GlyphRunList 有添加 glyph 的方法
  // glyphRunList->addGlyph(...);
  auto glyphShape = std::make_shared<GlyphShape>(glyphRunList);

  // 添加 Baseline::Compare 前置逻辑
  auto device = GLDevice::Make();
  auto context = device->lockContext();
  auto surface = Surface::Make(context, 400, 400);
  auto canvas = surface->getCanvas();

  Paint paint;
  paint.setColor(Color{1.f, 0.f, 0.f, 1.f});
  paint.setStyle(PaintStyle::Fill);

  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(glyphShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/GlyphShapeJson1"));

  std::string glyphJson = glyphShape->toJson();
  auto parsedGlyphShape = Shape::FromJson(glyphJson);
  ASSERT_NE(parsedGlyphShape, nullptr);
  ASSERT_EQ(parsedGlyphShape->type(), Shape::Type::Glyph);
  // 验证 GlyphRunList
  auto parsedGlyph = std::static_pointer_cast<GlyphShape>(parsedGlyphShape)->glyphRunList;
  ASSERT_EQ(glyphRunList, parsedGlyph);

  // 添加反序列化后的绘制逻辑
  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(parsedGlyphShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/GlyphShapeJson2"));

  device->unlock();
}

// 添加 EffectShape 的单元测试
TGFX_TEST(ShapeJsonTest, EffectShapeJson) {
  // 创建 EffectShape 并测试序列化
  Path path2;
  auto baseShape = std::make_shared<PathShape>(path2);
  std::shared_ptr<PathEffect> pathEffect = PathEffect::MakeCorner(0);
  auto effectShape = std::make_shared<EffectShape>(baseShape, pathEffect);

  // 添加 Baseline::Compare 前置逻辑
  auto device = GLDevice::Make();
  auto context = device->lockContext();
  auto surface = Surface::Make(context, 400, 400);
  auto canvas = surface->getCanvas();

  Paint paint;
  paint.setColor(Color{1.f, 0.f, 0.f, 1.f});
  paint.setStyle(PaintStyle::Fill);

  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(effectShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/EffectShapeJson1"));

  std::string effectJson = effectShape->toJson();
  auto parsedEffectShape = Shape::FromJson(effectJson);
  ASSERT_NE(parsedEffectShape, nullptr);
  ASSERT_EQ(parsedEffectShape->type(), Shape::Type::Effect);
  // 验证 Effect 属性
  auto parsedEffect = std::static_pointer_cast<EffectShape>(parsedEffectShape);
  ASSERT_EQ(parsedEffect->effect, pathEffect);
  ASSERT_EQ(parsedEffect->shape->type(), Shape::Type::Path);

  // 添加反序列化后的绘制逻辑
  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(parsedEffectShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/EffectShapeJson2"));

  device->unlock();
}

// 添加 AppendShape 的单元测试
TGFX_TEST(ShapeJsonTest, AppendShapeJson) {
  // 创建 AppendShape 并测试序列化
  Path path2;
  auto baseShape = std::make_shared<PathShape>(path2);
  auto secondShape = std::make_shared<PathShape>(path2);
  std::vector<std::shared_ptr<Shape>> shapes = {baseShape, secondShape};
  auto appendShape = std::make_shared<AppendShape>(std::move(shapes));

  // 添加 Baseline::Compare 前置逻辑
  auto device = GLDevice::Make();
  auto context = device->lockContext();
  auto surface = Surface::Make(context, 400, 400);
  auto canvas = surface->getCanvas();

  Paint paint;
  paint.setColor(Color{1.f, 0.f, 0.f, 1.f});
  paint.setStyle(PaintStyle::Fill);

  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(appendShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/AppendShapeJson1"));

  std::string appendJson = appendShape->toJson();
  auto parsedAppendShape = Shape::FromJson(appendJson);
  ASSERT_NE(parsedAppendShape, nullptr);
  ASSERT_EQ(parsedAppendShape->type(), Shape::Type::Append);
  // 验证 Append 属性
  auto parsedAppend = std::static_pointer_cast<AppendShape>(parsedAppendShape);
    ASSERT_EQ(parsedAppend->shapes.size(), 2u);
    ASSERT_EQ(parsedAppend->shapes[0]->type(), Shape::Type::Path);
    ASSERT_EQ(parsedAppend->shapes[1]->type(), Shape::Type::Path);


  // 添加反序列化后的绘制逻辑
  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawShape(parsedAppendShape, paint);
  EXPECT_TRUE(Baseline::Compare(surface, "ShapeJsonTest/AppendShapeJson2"));

  device->unlock();
}


}  // namespace tgfx
