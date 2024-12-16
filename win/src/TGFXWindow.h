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

#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>
#include <Windowsx.h>
#include <functional>
#include <memory>
#include <string>
#include "drawers/Drawer.h"
#include "tgfx/gpu/opengl/egl/EGLWindow.h"

namespace hello2d {

struct RectData {
  float size;
  float speed;
  tgfx::Rect rect;
};

//--------------------------------------

class TGFXWindow {
 public:
  TGFXWindow();
  virtual ~TGFXWindow();

  bool open();

 protected:
  //---------------
  bool attach();
  bool detach();

  void setTitle(const char* title) const;
  void drawRects(tgfx::Canvas* canvas);

  void initRects(int count);
  bool animateRects(long long ms);

 private:
  HWND windowHandle = nullptr;
  int lastDrawIndex = 0;
  std::shared_ptr<tgfx::EGLWindow> tgfxWindow = nullptr;
  std::shared_ptr<drawers::AppHost> appHost = nullptr;

  static WNDCLASS RegisterWindowClass();
  static LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) noexcept;

  LRESULT handleMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam) noexcept;

  void draw();
  void destroy();
  void centerAndShow();
  float getPixelRatio();
  void createAppHost();

  int fWidth;
  int fHeight;
  std::shared_ptr<tgfx::Device> fDevice;
  tgfx::Context* fContext;

  std::vector<RectData> fRects;
  bool fAnimateEnabled;
};
}  // namespace hello2d
