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

#include "TimelineController.h"
#include "TimelineContext.h"
#include "View.h"

TimelineController::TimelineController(TimelineView& view, tracy::Worker& worker, bool threading)
  : view(view)
  , worker(worker)
  , taskDispatch(
      threading ? (size_t)std::max( 0, ( (int)std::thread::hardware_concurrency() - 2 ) / 2 ) : 0
      , "Render")
{

}

TimelineController::~TimelineController() {

}

void TimelineController::firstFrameExpired() {
}

void TimelineController::begin() {
  items.clear();
}

void TimelineController::end(double pxns, const QPointF& wpos, bool vcenter, float yMin, float yMax, QPainter* painter) {
  const auto& viewData = view.getViewData();
  TimelineContext context;
  context.w = view.width() - 1;
  context.ty = 15;
  context.sty = 10;
  context.scale = 1;
  context.yMin = yMin;
  context.yMax = yMax;
  context.pxns = pxns;
  context.nspx = 1.0 / pxns;
  context.vStart = viewData.zvStart;
  context.vEnd = viewData.zvEnd;
  context.wpos = wpos;

  int yOffset = 0;
  for(auto& item: items) {
    if (item->isVisible()) {
      const auto yPos = wpos.y() + yOffset;
      const bool visible = firstFrame || (yPos < yMax && yPos + item->getHeight() >= yMin);
      item->preprocess(context, taskDispatch, visible, yPos);
    }
    yOffset += firstFrame ? 0 : item->getHeight();
  }
  taskDispatch.Sync();

  yOffset = 0;
  for(auto& item : items) {
    auto currentFrameItemHeight = item->getHeight();
    item->draw(firstFrame, context, yOffset, painter);
    if (firstFrame) {
      currentFrameItemHeight = item->getHeight();
    }
    yOffset += currentFrameItemHeight;
  }
}