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

#include <iostream>
#include <sstream>
#include <string>
#include "base/TGFXTest.h"
#include "gpu/opengl/GLCaps.h"
#include "gpu/opengl/GLUtil.h"
#include "gtest/gtest.h"
#include "tgfx/core/Color.h"
#include "tgfx/core/Paint.h"
#include "tgfx/core/Path.h"
#include "tgfx/core/Rect.h"
#include "tgfx/core/Size.h"
#include "tgfx/gpu/opengl/GLDevice.h"
#include "tgfx/svg/SVGExporter.h"
#include "utils/TestUtils.h"

namespace tgfx {

TGFX_TEST(SVGExportTest, PureColor) {
  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"200\" height=\"200\"><rect "
      "fill=\"#00F\" x=\"50\" y=\"50\" width=\"100\" height=\"100\"/></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  tgfx::Paint paint;
  paint.setColor(Color::Blue());

  std::stringstream SVGStream;
  auto exporter =
      SVGExporter::Make(SVGStream, context, Rect::MakeWH(200, 200), ExportingOptions(false, false));
  auto* canvas = exporter->getCanvas();
  canvas->drawRect(Rect::MakeXYWH(50, 50, 100, 100), paint);

  exporter->close();
  std::string SVGString = SVGStream.str();
  ASSERT_EQ(SVGString, compareString);
}

TGFX_TEST(SVGExportTest, OpacityColor) {
  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"200\" height=\"200\"><circle "
      "fill=\"#00007F\" fill-opacity=\"0.5\" cx=\"100\" cy=\"100\" r=\"100\"/></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  tgfx::Paint paint;
  paint.setColor(Color::Blue());
  paint.setAlpha(0.5f);

  std::stringstream SVGStream;
  auto exporter =
      SVGExporter::Make(SVGStream, context, Rect::MakeWH(200, 200), ExportingOptions(false, false));
  auto* canvas = exporter->getCanvas();

  canvas->drawCircle(100, 100, 100, paint);

  exporter->close();
  std::string SVGString = SVGStream.str();
  ASSERT_EQ(SVGString, compareString);
}

TGFX_TEST(SVGExportTest, LinearGradient) {
  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"200\" "
      "height=\"200\"><defs><linearGradient id=\"gradient_0\" gradientUnits=\"userSpaceOnUse\" "
      "x1=\"50\" y1=\"50\" x2=\"150\" y2=\"150\"><stop offset=\"0\" stop-color=\"#0F0\"/><stop "
      "offset=\"1\" stop-color=\"#000\"/></linearGradient></defs><circle fill=\"url(#gradient_0)\" "
      "cx=\"100\" cy=\"100\" r=\"100\"/></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  tgfx::Paint paint;
  auto shader = tgfx::Shader::MakeLinearGradient(
      tgfx::Point{50.f, 50.f}, tgfx::Point{150.f, 150.f},
      {tgfx::Color{0.f, 1.f, 0.f, 1.f}, tgfx::Color{0.f, 0.f, 0.f, 1.f}}, {});
  paint.setShader(shader);

  std::stringstream SVGStream;
  auto exporter =
      SVGExporter::Make(SVGStream, context, Rect::MakeWH(200, 200), ExportingOptions(false, false));
  auto* canvas = exporter->getCanvas();

  canvas->drawCircle(100, 100, 100, paint);

  exporter->close();
  std::string SVGString = SVGStream.str();
  ASSERT_EQ(SVGString, compareString);
}

TGFX_TEST(SVGExportTest, RadialGradient) {
  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"200\" "
      "height=\"200\"><defs><radialGradient id=\"gradient_0\" gradientUnits=\"userSpaceOnUse\" "
      "r=\"50\" cx=\"100\" cy=\"100\"><stop offset=\"0\" stop-color=\"#F00\"/><stop offset=\"0.5\" "
      "stop-color=\"#00F\"/><stop offset=\"1\" stop-color=\"#000\"/></radialGradient></defs><rect "
      "fill=\"url(#gradient_0)\" x=\"50\" y=\"50\" width=\"100\" height=\"100\"/></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  tgfx::Paint paint;
  tgfx::Point center{100.f, 100.f};
  auto shader = tgfx::Shader::MakeRadialGradient(
      center, 50, {tgfx::Color::Red(), tgfx::Color::Blue(), tgfx::Color::Black()}, {0, 0.5, 1.0});
  paint.setShader(shader);

  std::stringstream SVGStream;
  auto exporter =
      SVGExporter::Make(SVGStream, context, Rect::MakeWH(200, 200), ExportingOptions(false, false));
  auto* canvas = exporter->getCanvas();

  canvas->drawRect(Rect::MakeXYWH(50, 50, 100, 100), paint);

  exporter->close();
  std::string SVGString = SVGStream.str();
  ASSERT_EQ(SVGString, compareString);
}

TGFX_TEST(SVGExportTest, UnsupportedGradient) {

  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"200\" "
      "height=\"200\"><defs><linearGradient id=\"gradient_0\" gradientUnits=\"objectBoundingBox\" "
      "x1=\"0\" y1=\"0\" x2=\"1\" y2=\"0\"><stop offset=\"0\" stop-color=\"#F00\"/><stop "
      "offset=\"0.5\" stop-color=\"#00F\"/><stop offset=\"1\" "
      "stop-color=\"#000\"/></linearGradient></defs><rect fill=\"url(#gradient_0)\" x=\"50\" "
      "y=\"50\" width=\"100\" height=\"100\"/></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  tgfx::Paint paint;
  tgfx::Point center{100.f, 100.f};
  auto shader = tgfx::Shader::MakeConicGradient(
      center, 0, 360, {tgfx::Color::Red(), tgfx::Color::Blue(), tgfx::Color::Black()},
      {0, 0.5, 1.0});
  paint.setShader(shader);

  std::stringstream SVGStream;
  auto exporter =
      SVGExporter::Make(SVGStream, context, Rect::MakeWH(200, 200), ExportingOptions(false, false));
  auto* canvas = exporter->getCanvas();

  canvas->drawRect(Rect::MakeXYWH(50, 50, 100, 100), paint);

  exporter->close();
  std::string SVGString = SVGStream.str();
  ASSERT_EQ(SVGString, compareString);
}

TGFX_TEST(SVGExportTest, BlendMode) {
  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"200\" height=\"200\"><rect "
      "fill=\"#FFF\" width=\"100\" height=\"100\"/><rect fill=\"#F00\" fill-opacity=\"1\" "
      "style=\"mix-blend-mode:difference\" x=\"50\" y=\"50\" width=\"100\" height=\"100\"/></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  Paint paintBackground;
  paintBackground.setColor(Color::White());

  Paint paint;
  paint.setColor(Color::Red());
  paint.setBlendMode(BlendMode::Difference);

  std::stringstream SVGStream;
  SVGExporter exporter(SVGStream, context, Rect::MakeWH(200, 200), ExportingOptions(false, false));
  auto* canvas = exporter.getCanvas();

  canvas->drawRect(tgfx::Rect::MakeXYWH(0, 0, 100, 100), paintBackground);
  canvas->drawRect(tgfx::Rect::MakeXYWH(50, 50, 100, 100), paint);

  exporter.close();
  std::string SVGString = SVGStream.str();
  ASSERT_EQ(SVGString, compareString);
}

TGFX_TEST(SVGExportTest, StrokeWidth) {
  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"200\" height=\"200\"><path "
      "fill=\"#F00\" d=\"M47.5 47.5L152.5 47.5L152.5 152.5L47.5 152.5L47.5 47.5ZM52.5 52.5L52.5 "
      "147.5L147.5 147.5L147.5 52.5L52.5 52.5Z\"/></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  Paint paint;
  paint.setColor(Color::Red());
  paint.setStyle(PaintStyle::Stroke);
  paint.setStrokeWidth(5);

  std::stringstream SVGStream;
  auto exporter =
      SVGExporter::Make(SVGStream, context, Rect::MakeWH(200, 200), ExportingOptions(false, false));
  auto* canvas = exporter->getCanvas();

  canvas->drawRect(tgfx::Rect::MakeXYWH(50, 50, 100, 100), paint);

  exporter->close();
  std::string SVGString = SVGStream.str();
  ASSERT_EQ(SVGString, compareString);
}

TGFX_TEST(SVGExportTest, SimpleTextAsText) {
  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"400\" height=\"200\"><text "
      "fill=\"#F00\" transform=\"matrix(1 0 0 1 0 80)\" font-size=\"50\" font-family=\"Noto Serif "
      "SC\" x=\"0, 43, 70, 86, 102, 132, 145, 178, 215, 246, \" y=\"0, \">Hello TGFX</text></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  auto typeface =
      Typeface::MakeFromPath(ProjectPath::Absolute("resources/font/NotoSerifSC-Regular.otf"));
  Font font(typeface, 50.f);
  Paint paint;
  paint.setColor(Color::Red());

  std::stringstream SVGStream;
  auto exporter =
      SVGExporter::Make(SVGStream, context, Rect::MakeWH(400, 200), ExportingOptions(false, false));
  auto* canvas = exporter->getCanvas();

  canvas->drawSimpleText("Hello TGFX", 0, 80, font, paint);

  exporter->close();
  std::string SVGString = SVGStream.str();
  ASSERT_EQ(SVGString, compareString);
}

TGFX_TEST(SVGExportTest, SimpleTextAsPath) {
  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"400\" height=\"200\"><path "
      "fill=\"#F00\" transform=\"matrix(1 0 0 1 0 80)\" d=\"M5 -0L9.6406 0L9.6406 -17L26.75 "
      "-17L26.75 0L31.3438 0L31.3438 -37L26.75 -37L26.75 -21L9.6406 -21L9.6406 -37L5 -37L5 "
      "0ZM40.5938 0L45.1406 0L45.1406 -27L40.5938 -27L40.5938 0ZM42.8906 -32.9844C44.6875 -32.9844 "
      "45.9375 -34.1875 45.9375 -36.0469C45.9375 -37.7969 44.6875 -39 42.8906 -39C41.0938 -39 "
      "39.8438 -37.7969 39.8438 -36.0469C39.8438 -34.1875 41.0938 -32.9844 42.8906 "
      "-32.9844Z\"/></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  auto typeface =
      Typeface::MakeFromPath(ProjectPath::Absolute("resources/font/NotoSansSC-Regular.otf"));
  Font font(typeface, 50.f);
  Paint paint;
  paint.setColor(Color::Red());

  std::stringstream SVGStream;
  auto exporter =
      SVGExporter::Make(SVGStream, context, Rect::MakeWH(400, 200), ExportingOptions(true, false));
  auto* canvas = exporter->getCanvas();

  canvas->drawSimpleText("Hi", 0, 80, font, paint);

  exporter->close();
  std::string SVGString = SVGStream.str();
  std::cout << SVGString << std::endl;
  std::cout << compareString << std::endl;
  ASSERT_EQ(SVGString, compareString);
}

TGFX_TEST(SVGExportTest, EmojiText) {
  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"400\" height=\"200\"><text "
      "fill=\"#F00\" transform=\"matrix(1 0 0 1 0 80)\" font-size=\"50\" font-family=\"Noto Color "
      "Emoji\" x=\"0, 62.3906, 124.7812, 187.1719, 249.5625, 311.9531, \" y=\"0, "
      "\">🤡👻🐠🤩😃🤪</text></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  auto typeface =
      Typeface::MakeFromPath(ProjectPath::Absolute("resources/font/NotoColorEmoji.ttf"));
  Font font(typeface, 50.f);
  Paint paint;
  paint.setColor(Color::Red());

  std::stringstream SVGStream;
  auto exporter =
      SVGExporter::Make(SVGStream, context, Rect::MakeWH(400, 200), ExportingOptions(false, false));
  auto* canvas = exporter->getCanvas();

  canvas->drawSimpleText("🤡👻🐠🤩😃🤪", 0, 80, font, paint);

  exporter->close();
  std::string SVGString = SVGStream.str();
  ASSERT_EQ(SVGString, compareString);
}

TGFX_TEST(SVGExportTest, ClipState) {
  std::string compareString =
      "<?xml version=\"1.0\" encoding=\"utf-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" "
      "xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"300\" height=\"300\"><clipPath "
      "id=\"clip_0\"><rect width=\"100\" height=\"100\"/></clipPath><g "
      "clip-path=\"url(#clip_0)\"><rect fill=\"#F00\" width=\"200\" height=\"200\"/></g><clipPath "
      "id=\"clip_1\"><circle cx=\"150\" cy=\"150\" r=\"50\"/></clipPath><g "
      "clip-path=\"url(#clip_1)\"><rect fill=\"#0F0\" x=\"100\" y=\"100\" width=\"200\" "
      "height=\"200\"/></g><rect fill=\"#00F\" x=\"200\" y=\"200\" width=\"100\" "
      "height=\"100\"/></svg>";

  ContextScope scope;
  auto* context = scope.getContext();
  ASSERT_TRUE(context != nullptr);

  std::stringstream SVGStream;
  auto exporter =
      SVGExporter::Make(SVGStream, context, Rect::MakeWH(300, 300), ExportingOptions(false, false));
  auto* canvas = exporter->getCanvas();

  {
    Paint paint;
    paint.setColor(Color::Red());
    canvas->save();
    canvas->clipRect(Rect::MakeXYWH(0, 0, 100, 100));
    canvas->drawRect(Rect::MakeXYWH(0, 0, 200, 200), paint);
    canvas->restore();

    paint.setColor(Color::Green());
    canvas->save();
    Path path;
    path.addOval(Rect::MakeXYWH(100, 100, 100, 100));
    canvas->clipPath(path);
    canvas->drawRect(Rect::MakeXYWH(100, 100, 200, 200), paint);
    canvas->restore();

    paint.setColor(Color::Blue());
    canvas->save();
    canvas->drawRect(Rect::MakeXYWH(200, 200, 100, 100), paint);
    canvas->restore();
  }

  exporter->close();
  std::string SVGString = SVGStream.str();
  ASSERT_EQ(SVGString, compareString);
}

}  // namespace tgfx