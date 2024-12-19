#include <gtest/gtest.h>
#include <tgfx/layers/record/Recorder.h>
#include <vector>
#include "core/utils/Profiling.h"
#include "tgfx/core/Path.h"
#include "tgfx/layers/Layer.h"
#include "tgfx/layers/ShapeLayer.h"
#include "utils/TestUtils.h"
#include "utils/common.h"

namespace tgfx {
TEST(PathTest, SerializeDeserialize) {
  auto device = GLDevice::Make();
  auto context = device->lockContext();
  auto surface = Surface::Make(context, 400, 400);
  auto canvas = surface->getCanvas();

  Paint strokePaint;
  strokePaint.setColor(Color{1.f, 0.f, 0.f, 1.f});
  strokePaint.setStrokeWidth(2.f);
  strokePaint.setStyle(PaintStyle::Stroke);
  // -----------------------------------------------------
  // 创建并设置一个路径
  Path originalPath;
  originalPath.moveTo(0.0f, 0.0f);
  originalPath.lineTo(100.0f, 0.0f);
  originalPath.lineTo(100.0f, 100.0f);
  originalPath.close();

  // -----------------------------------------------------

  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawPath(originalPath, strokePaint);
  EXPECT_TRUE(Baseline::Compare(surface, "PathTest/SerializeDeserialize1"));

  // -----------------------------------------------------
  // 序列化路径
  auto jsonStr = originalPath.toJson();
  std::cout << jsonStr << std::endl;
  // 反序列化到新路径
  Path deserializedPath;
  deserializedPath.fromJson(jsonStr);

  // -----------------------------------------------------
  canvas->clearRect(Rect::MakeWH(surface->width(), surface->height()), Color::White());
  canvas->drawPath(deserializedPath, strokePaint);
  EXPECT_TRUE(Baseline::Compare(surface, "PathTest/SerializeDeserialize2"));
  device->unlock();
}

TEST(PathTest, SerializeDeserializeEmptyPath) {
  // 创建一个空路径
  Path originalPath;

  // 序列化路径
  std::vector<uint8_t> serializedData = originalPath.serialize();

  // 反序列化到新路径
  Path deserializedPath;
  bool success = deserializedPath.deserialize(serializedData);
  ASSERT_TRUE(success) << "Deserialization failed for empty path";

  // 比较原始路径和反序列化后的路径
    ASSERT_EQ(originalPath, deserializedPath) << "Original and deserialized empty paths do not match";
}

}