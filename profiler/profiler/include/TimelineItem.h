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

#include <TracyTaskDispatch.hpp>
#include "TimelineContext.h"
#include <QPainter>
#include "TracyWorker.hpp"

class TimelineView;

class TimelineItem {
public:
  TimelineItem(TimelineView& view, tracy::Worker& worker, const void* key);
  virtual ~TimelineItem() = default;

  void draw(bool firstFrame, const TimelineContext ctx, int yOffset, QPainter* painter);
  virtual void preprocess(const TimelineContext& ctx, tracy::TaskDispatch& td, bool visible, int yPos) = 0;
  virtual uint32_t headerColor() const = 0;
  virtual uint32_t headerColorInactive() const = 0;
  virtual uint32_t headlineColor() const = 0;
  virtual const char* headerLable() const = 0;

  virtual void setVisible(bool v) { visible = v; }
  virtual bool isVisible() { return visible; }
  virtual bool isEmpty() { return false; }
  virtual void drawOverlay(const QPointF& ul, const QPointF& dr) {}

  int getHeight() const {return height;}
protected:
  virtual void headerToolTip(const char* lable) const {}
  virtual void headerExtraContents( const TimelineContext& ctx, int offset, float labelWidth ) {};

  virtual bool drawContent(const TimelineContext& ctx, int& offset, QPainter* painter) = 0;
  virtual void drawFinished() {}
  void adjustThreadHeight(bool firstFrame, int yBegin, int yEnd);
private:
  const void* key;
  int height;
protected:
  bool visible;
  bool showFull;
  tracy::Worker& worker;
  TimelineView& timelineView;
};