/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Tencent is pleased to support the open source community by making tgfx available.
//
//  Copyright (C) 2023 THL A29 Limited, a Tencent company. All rights reserved.
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

#include <native_buffer/native_buffer.h>
#include "core/PixelBuffer.h"
#include "utils/PixelFormatUtil.h"

namespace tgfx {
std::shared_ptr<ImageBuffer> ImageBuffer::MakeFrom(HardwareBufferRef hardwareBuffer,
                                                   YUVColorSpace) {
  return PixelBuffer::MakeFrom(hardwareBuffer);
}

bool HardwareBufferCheck(HardwareBufferRef buffer) {
  if (!HardwareBufferAvailable()) {
    return false;
  }
  return buffer != nullptr;
}

HardwareBufferRef HardwareBufferAllocate(int width, int height, bool alphaOnly) {
  if (!HardwareBufferAvailable() || alphaOnly) {
    return nullptr;
  }
  OH_NativeBuffer_Config config = {width, height, NATIVEBUFFER_PIXEL_FMT_RGBA_8888,
                                   NATIVEBUFFER_USAGE_CPU_READ | NATIVEBUFFER_USAGE_CPU_WRITE |
                                       NATIVEBUFFER_USAGE_HW_RENDER | NATIVEBUFFER_USAGE_HW_TEXTURE,
                                   0};
  return OH_NativeBuffer_Alloc(&config);
}

HardwareBufferRef HardwareBufferRetain(HardwareBufferRef buffer) {
  if (buffer != nullptr) {
    OH_NativeBuffer_Reference(buffer);
  }
  return buffer;
}

void HardwareBufferRelease(HardwareBufferRef buffer) {
  if (buffer != nullptr) {
    OH_NativeBuffer_Unreference(buffer);
  }
}

void* HardwareBufferLock(HardwareBufferRef buffer) {
  if (buffer == nullptr) {
    return nullptr;
  }
  void* pixels = nullptr;
  auto result = OH_NativeBuffer_Map(buffer, &pixels);
  if (result != 0) {
    return nullptr;
  }
  return pixels;
}

void HardwareBufferUnlock(HardwareBufferRef buffer) {
  if (buffer != nullptr) {
    OH_NativeBuffer_Unmap(buffer);
  }
}

ImageInfo HardwareBufferGetInfo(HardwareBufferRef buffer) {
  if (!HardwareBufferAvailable() || buffer == nullptr) {
    return {};
  }
  OH_NativeBuffer_Config config;
  OH_NativeBuffer_GetConfig(buffer, &config);
  auto colorType = ColorType::Unknown;
  auto alphaType = AlphaType::Premultiplied;
  switch (config.format) {
    case NATIVEBUFFER_PIXEL_FMT_RGBA_8888:
      colorType = ColorType::RGBA_8888;
      break;
    case NATIVEBUFFER_PIXEL_FMT_RGBX_8888:
      colorType = ColorType::RGBA_8888;
      alphaType = AlphaType::Opaque;
      break;
    default:
      break;
  }
  auto bytesPerPixel = ImageInfo::GetBytesPerPixel(colorType);
  return ImageInfo::Make(config.width, config.height, colorType,
                         alphaType, static_cast<size_t>(config.stride) * bytesPerPixel);
}

PixelFormat HardwareBufferGetPixelFormat(HardwareBufferRef buffer) {
  auto info = HardwareBufferGetInfo(buffer);
  if (info.isEmpty()) {
    return PixelFormat::Unknown;
  }
  return ColorTypeToPixelFormat(info.colorType());
}
}  // namespace tgfx