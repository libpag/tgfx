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

#include "TGFXWindow.h"
#include <shellscalingapi.h>
#include <filesystem>
#include <random>

namespace hello2d {

constexpr int kRectCount = 500;

static constexpr LPCWSTR ClassName = L"TGFXWindow";

TGFXWindow::TGFXWindow() {
  createAppHost();

  fAnimateEnabled = true;
}

TGFXWindow::~TGFXWindow() {
  destroy();
}

bool TGFXWindow::open() {
  destroy();
  WNDCLASS windowClass = RegisterWindowClass();
  auto pixelRatio = getPixelRatio();
  int initWidth = static_cast<int>(pixelRatio * 1024);
  int initHeight = static_cast<int>(pixelRatio * 720);
  windowHandle =
      CreateWindowEx(WS_EX_APPWINDOW, windowClass.lpszClassName, L"Hello2D", WS_OVERLAPPEDWINDOW, 0,
                     0, initWidth, initHeight, nullptr, nullptr, windowClass.hInstance, this);

  if (windowHandle == nullptr) {
    return false;
  }
  SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
  centerAndShow();

  attach();
  initRects(kRectCount);

  return true;
}

WNDCLASS TGFXWindow::RegisterWindowClass() {
  auto hInstance = GetModuleHandle(nullptr);
  WNDCLASS windowClass{};
  windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
  windowClass.lpszClassName = ClassName;
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = hInstance;
  windowClass.hIcon = LoadIcon(hInstance, L"IDI_ICON1");
  windowClass.hbrBackground = nullptr;
  windowClass.lpszMenuName = nullptr;
  windowClass.lpfnWndProc = WndProc;
  RegisterClass(&windowClass);
  return windowClass;
}

LRESULT CALLBACK TGFXWindow::WndProc(HWND window, UINT message, WPARAM wparam,
                                     LPARAM lparam) noexcept {
  auto tgfxWindow = reinterpret_cast<TGFXWindow*>(GetWindowLongPtr(window, GWLP_USERDATA));
  if (tgfxWindow != nullptr) {
    return tgfxWindow->handleMessage(window, message, wparam, lparam);
  }
  return DefWindowProc(window, message, wparam, lparam);
}

LRESULT TGFXWindow::handleMessage(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) noexcept {
  LRESULT result = 0;
  switch (message) {
    case WM_DESTROY:
      destroy();
      PostQuitMessage(0);
      break;
    case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(windowHandle, &ps);
      draw();
      EndPaint(windowHandle, &ps);
      break;
    }
    case WM_LBUTTONDOWN:
      //lastDrawIndex++;
      //::InvalidateRect(windowHandle, nullptr, TRUE);
      break;
    default:
      result = DefWindowProc(windowHandle, message, wparam, lparam);
      break;
  }
  return result;
}

void TGFXWindow::destroy() {
  if (windowHandle) {
    detach();

    DestroyWindow(windowHandle);
    windowHandle = nullptr;
    UnregisterClass(ClassName, nullptr);
  }
}

void TGFXWindow::centerAndShow() {
  if ((GetWindowStyle(windowHandle) & WS_CHILD) != 0) {
    return;
  }
  RECT rcDlg = {0};
  ::GetWindowRect(windowHandle, &rcDlg);
  RECT rcArea = {0};
  RECT rcCenter = {0};
  HWND hWnd = windowHandle;
  HWND hWndCenter = ::GetWindowOwner(windowHandle);
  if (hWndCenter != nullptr) {
    hWnd = hWndCenter;
  }

  MONITORINFO oMonitor = {};
  oMonitor.cbSize = sizeof(oMonitor);
  ::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &oMonitor);
  rcArea = oMonitor.rcWork;

  if (hWndCenter == nullptr) {
    rcCenter = rcArea;
  } else {
    ::GetWindowRect(hWndCenter, &rcCenter);
  }

  int DlgWidth = rcDlg.right - rcDlg.left;
  int DlgHeight = rcDlg.bottom - rcDlg.top;

  // Find dialog's upper left based on rcCenter
  int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
  int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

  // The dialog is outside the screen, move it inside
  if (xLeft < rcArea.left) {
    if (xLeft < 0) {
      xLeft = GetSystemMetrics(SM_CXSCREEN) / 2 - DlgWidth / 2;
    } else {
      xLeft = rcArea.left;
    }
  } else if (xLeft + DlgWidth > rcArea.right) {
    xLeft = rcArea.right - DlgWidth;
  }

  if (yTop < rcArea.top) {
    if (yTop < 0) {
      yTop = GetSystemMetrics(SM_CYSCREEN) / 2 - DlgHeight / 2;
    } else {
      yTop = rcArea.top;
    }

  } else if (yTop + DlgHeight > rcArea.bottom) {
    yTop = rcArea.bottom - DlgHeight;
  }
  ::SetWindowPos(windowHandle, nullptr, xLeft, yTop, -1, -1,
                 SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

float TGFXWindow::getPixelRatio() {
#if WINVER >= 0x0603  // Windows 8.1
  HMONITOR monitor = nullptr;
  if (windowHandle != nullptr) {
    monitor = ::MonitorFromWindow(windowHandle, MONITOR_DEFAULTTONEAREST);
  } else {
    monitor = ::MonitorFromPoint(POINT{0, 0}, MONITOR_DEFAULTTOPRIMARY);
  }
  UINT dpiX = 96;
  UINT dpiY = 96;
  GetDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
  return static_cast<float>(dpiX) / 96.0f;
#else
  return 1.0f;
#endif
}

void TGFXWindow::createAppHost() {
  appHost = std::make_unique<drawers::AppHost>();
  std::filesystem::path filePath = __FILE__;
  auto rootPath = filePath.parent_path().parent_path().parent_path().string();
  auto imagePath = rootPath + R"(\resources\assets\bridge.jpg)";
  auto image = tgfx::Image::MakeFromFile(imagePath);
  appHost->addImage("bridge", image);
  auto typeface = tgfx::Typeface::MakeFromName("Microsoft YaHei", "");
  appHost->addTypeface("default", typeface);
  auto emojiPath = rootPath + R"(\resources\font\NotoColorEmoji.ttf)";
  typeface = tgfx::Typeface::MakeFromPath(emojiPath);
  appHost->addTypeface("emoji", typeface);
}

void TGFXWindow::setTitle(const char* title) const {
  SetWindowTextA(windowHandle, title);
}

bool TGFXWindow::attach() {
  if (!tgfxWindow) {
    tgfxWindow = tgfx::EGLWindow::MakeFrom(windowHandle);
  }
  if (tgfxWindow == nullptr) {
    return false;
  }

  RECT rect;
  GetClientRect(windowHandle, &rect);
  fWidth = static_cast<int>(rect.right - rect.left);
  fHeight = static_cast<int>(rect.bottom - rect.top);
  auto pixelRatio = getPixelRatio();
  auto sizeChanged = appHost->updateScreen(fWidth, fHeight, pixelRatio);
  if (sizeChanged) {
    tgfxWindow->invalidSize();
  }

  fDevice = tgfxWindow->getDevice();
  if (fDevice == nullptr) {
    return false;
  }
  fContext = fDevice->lockContext();
  if (fContext == nullptr) {
    return false;
  }

  return true;
}

bool TGFXWindow::detach() {
  if (!fDevice) return false;
  fDevice->unlock();
  return true;
}

static int frames = 0;
static long long startMs = 0;

void TGFXWindow::draw() {
  auto surface = tgfxWindow->getSurface(fContext);
  if (surface == nullptr) {
    //fDevice->unlock();
    return;
  }

  auto canvas = surface->getCanvas();
  canvas->clear();
  canvas->save();

  drawRects(canvas);

  //auto numDrawers = drawers::Drawer::kRectCount() - 1;
  //auto index = (lastDrawIndex % numDrawers) + 1;
  //auto drawer = drawers::Drawer::GetByName("GridBackground");
  //drawer->draw(canvas, appHost.get());
  //drawer = drawers::Drawer::GetByIndex(index);
  //drawer->draw(canvas, appHost.get());

  canvas->restore();
  fContext->flushAndSubmit();
  tgfxWindow->present(fContext);

  if (fAnimateEnabled) {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto ms = now_ms.time_since_epoch().count();

    if (frames <= 0) startMs = ms;
    ++frames;

    auto sec = (ms - startMs) / 1000;
    if (sec >= 1) {
      int fps = std::round(frames / sec);
      std::string str = "Count :" + std::to_string(kRectCount) + ", MS : " + std::to_string(ms) +
                        ", FPS : " + std::to_string(fps);
      setTitle(str.c_str());

      frames = 0;
      startMs = ms;
    }

    if (animateRects(ms)) {
      ::InvalidateRect(windowHandle, nullptr, false);
    }
  }
}

void TGFXWindow::drawRects(tgfx::Canvas* canvas) {
  tgfx::Paint paint1;
  paint1.setColor(tgfx::Color::Red());
//  paint1.setStyle(tgfx::PaintStyle::Stroke);
  paint1.setAntiAlias(false);

  tgfx::Paint paint2;
  paint2.setColor(tgfx::Color::Green());
//  paint2.setStyle(tgfx::PaintStyle::Stroke);
  paint2.setAntiAlias(false);

  tgfx::Paint paint3;
  paint3.setColor(tgfx::Color::Blue());
//  paint3.setStyle(tgfx::PaintStyle::Stroke);
  paint3.setAntiAlias(false);

  int n = 0;
  for (int i = 0; i < fRects.size(); ++i) {
    RectData* r = &fRects[i];

    int m = n++;
    if (m == 0) canvas->drawRect(r->rect, paint1);
    else if (m == 1)
      canvas->drawRect(r->rect, paint2);
    else if (m == 2)
      canvas->drawRect(r->rect, paint3);

    if (n > 2) n = 0;
  }
}

void TGFXWindow::initRects(int count) {
  RECT rect;
  GetClientRect(windowHandle, &rect);
  auto width = static_cast<int>(rect.right - rect.left);
  auto height = static_cast<int>(rect.bottom - rect.top);

  fRects.clear();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  for (int i = 0; i < count; ++i) {
    float x = dis(gen) * width;
    float y = dis(gen) * height;
    float size = 10.0 + dis(gen) * 40.0;
    float speed = 1.0 + dis(gen);
    tgfx::Rect rect = tgfx::Rect::MakeXYWH(x, y, size, size);

    fRects.push_back(RectData{size, speed, rect});
  }
}

bool TGFXWindow::animateRects(long long ms) {

  for (int i = 0; i < fRects.size(); ++i) {
    RectData* r = &fRects[i];
    float x = r->rect.x();

    x -= r->speed;
    if (x + r->size < 0) {
      x = fWidth + r->size;
    }

    r->rect.setXYWH(x, r->rect.y(), r->size, r->size);
  }

  return true;
}

}  // namespace hello2d