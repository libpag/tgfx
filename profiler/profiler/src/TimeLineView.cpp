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

#include "TimelineView.h"
#include <qevent.h>
#include <TracyPrint.hpp>
#include <cinttypes>
#include <iterator>
#include <iostream>
#include <src/profiler/TracyColor.hpp>
#include "TimelineItemThread.h"

constexpr float MinVisSize = 3;
constexpr float MinFrameSize = 5;

static tracy_force_inline uint32_t getColorMuted(uint32_t color, bool active)
{
  if(active)
  {
    return 0xFF000000 | color;
  }
  else
  {
    return 0x66000000 | color;
  }
}


TimelineView::TimelineView(tracy::Worker& worker, ViewData& viewData, bool threadedRendering, QWidget* parent)
  : worker(worker)
  , viewData(viewData)
  , frameData(worker.GetFramesBase())
  , timelineController(*this, worker, threadedRendering) {
  update();
}

TimelineView::~TimelineView() {

}


uint32_t TimelineView::getRawSrcLocColor(const tracy::SourceLocation& srcloc, int depth) {
  auto namehash = srcloc.namehash;
  if(namehash == 0 && srcloc.function.active)
  {
    const auto f = worker.GetString(srcloc.function);
    namehash = tracy::charutil::hash(f);
    if(namehash == 0) namehash++;
    srcloc.namehash = namehash;
  }
  if(namehash == 0)
  {
    return tracy::GetHsvColor(uint64_t(&srcloc), depth);
  }
  else
  {
    return tracy::GetHsvColor(namehash, depth);
  }
}

uint32_t TimelineView::getZoneColor(const tracy::ZoneEvent& ev, uint64_t thread, int depth) {
  const auto sl = ev.SrcLoc();
  const auto& srcloc = worker.GetSourceLocation(sl);
  if(!viewData.forceColors)
  {
    if(worker.HasZoneExtra(ev))
    {
      const auto custom_color = worker.GetZoneExtra(ev).color.Val();
      if(custom_color != 0) return custom_color | 0xFF000000;
    }
    const auto color = srcloc.color;
    if(color != 0) return color | 0xFF000000;
  }
  switch(viewData.dynamicColors)
  {
    case 0:
      return 0xFFCC5555;
    case 1:
      return tracy::GetHsvColor(thread, depth);
    case 2:
      return getRawSrcLocColor(srcloc, depth);
    default:
      assert(false);
    return 0;
  }
}

TimelineView::ZoneColorData TimelineView::getZoneColorData(const tracy::ZoneEvent& ev, uint64_t thread, int depth, uint32_t inheritedColor) {
  ZoneColorData ret;
  const auto& srcloc = ev.SrcLoc();

  const auto color = inheritedColor ? inheritedColor : getZoneColor(ev, thread, depth);
  ret.color = color;
  ret.accentColor = tracy::HighlightColor(color);
  ret.thickness = 1.f;
  ret.highlight = false;
  return ret;
}

uint64_t TimelineView::getFrameNumber(const tracy::FrameData& fd, int i) const {
  if(fd.name == 0) {
    const auto offset = worker.GetFrameOffset();
    if(offset == 0) {
      return i;
    }
    else {
      return i + offset - 1;
    }
  }
  else {
    return i + 1;
  }
}

const char* TimelineView::getFrameSetName(const tracy::FrameData& fd) const {
  return getFrameSetName(fd, worker);
}

const char* TimelineView::getFrameSetName(const tracy::FrameData& fd, const tracy::Worker& worker) const {
  enum { Pool = 4 };
  static char bufpool[Pool][64];
  static int bufsel = 0;

  if(fd.name == 0)
  {
    return "Frames";
  }
  else if(fd.name >> 63 != 0)
  {
    char* buf = bufpool[bufsel];
    bufsel = (bufsel + 1) % Pool;
    sprintf(buf, "[%" PRIu32 "] Vsync", uint32_t(fd.name));
    return buf;
  }
  else
  {
    return worker.GetString(fd.name);
  }
}

const char* TimelineView::getFrameText(const tracy::FrameData& fd, int i, uint64_t ftime) const {
  const auto fnum = getFrameNumber(fd, i);
  static char buf[1024];
  if(fd.name == 0)
  {
    if(i == 0)
    {
      sprintf(buf, "Tracy init (%s)", tracy::TimeToString(ftime));
    }
    else if(i != 1 || !worker.IsOnDemand())
    {
      sprintf(buf, "Frame %s (%s)", tracy::RealToString(fnum), tracy::TimeToString(ftime));
    }
    else
    {
      sprintf(buf, "Missed frames (%s)", tracy::TimeToString(ftime));
    }
  }
  else
  {
    sprintf(buf, "%s %s (%s)", getFrameSetName(fd), tracy::RealToString(fnum), tracy::TimeToString(ftime));
  }
  return buf;
}

void TimelineView::drawMouseLine(QPainter* painter) {

}

void TimelineView::drawTimelineFrames(QPainter* painter, tracy::FrameData& fd, int& yMin) {
  const std::pair<int, int> zrange = worker.GetFrameRange(fd, viewData.zvStart, viewData.zvEnd);
  if(zrange.first < 0) return;
  if(worker.GetFrameBegin(fd, zrange.first) > viewData.zvEnd
    || worker.GetFrameEnd(fd, zrange.second) < viewData.zvStart) {
    return;
  }
  const auto wpos = QPointF(0.f, 0.f);
  const auto dpos = wpos + QPointF(0.5f, 0.5f);
  const auto w = width();
  const auto wh = height();
  const auto ty = 15;
  const auto ty025 = ty * 0.25f;
  const auto ty05 = round(ty * 0.5f);

  yMin += ty;

  auto timespan = viewData.zvEnd - viewData.zvStart;
  auto pxns = w / double(timespan);

  const auto nspx = 1.0 / pxns;

  int64_t prev = -1;
  int64_t prevEnd = -1;
  int64_t endPos = -1;
  bool tooltipDisplayed = false;
  const auto activeFrameSet = frameData == &fd;
  const int64_t frameTarget = (activeFrameSet && viewData.drawFrameTargets) ? 1000000000ll / viewData.frameTarget : std::numeric_limits<int64_t>::max();

  const auto inactiveColor = getColorMuted(0x888888, activeFrameSet);
  const auto activeColor = getColorMuted(0xFFFFFF, activeFrameSet);
  const auto redColor = getColorMuted(0x4444FF, activeFrameSet);

  int i = zrange.first;
  auto x1 = (worker.GetFrameBegin(fd, i) - viewData.zvStart) * pxns;
  while(i < zrange.second) {
    const auto ftime = worker.GetFrameTime(fd, i);
    const auto fbegin = worker.GetFrameBegin(fd, i);
    const auto fend = worker.GetFrameEnd(fd, i);
    const auto fsz = pxns * ftime;
    if (hoverData.hover) {

    }
    if (fsz < MinFrameSize) {
      if (!fd.continuous && prev != -1) {
        if((fbegin - prevEnd) * pxns >= MinFrameSize)
        {
          // drawImage(wpos + ImVec2(0, ty05), (prev - viewData.zvStart) * pxns, (prevEnd - viewData.zvStart) * pxns, ty025, inactiveColor);
          prev = -1;
        }
        else
        {
          prevEnd = std::max<int64_t>(fend, fbegin + MinFrameSize * nspx);
        }
      }
      if(prev == -1)
      {
        prev = fbegin;
        prevEnd = std::max<int64_t>(fend, fbegin + MinFrameSize * nspx);
      }

      const auto begin = fd.frames.begin() + i;
      const auto end = fd.frames.begin() + zrange.second;
      auto it = std::lower_bound(begin, end, int64_t(fbegin + MinVisSize * nspx),
        [this, &fd] (const auto& l, const auto& r) {
          // return worker.GetFrameEnd(fd, std::distance(fd.frames.begin(), &l)) < r;
          return worker.GetFrameEnd(fd, fd.frames.begin() - &l) < r;
        });
      if(it == begin) ++it;
      i += std::distance(begin, it);
      continue;
    }

    if(prev != -1)
    {
      if(fd.continuous)
      {
        // drawImage(draw, wpos + ImVec2(0, ty05), (prev - viewData.zvStart) * pxns, (fbegin - viewData.zvStart) * pxns, ty025, inactiveColor);
      }
      else
      {
        // drawImage(draw, wpos + ImVec2(0, ty05), (prev - viewData.zvStart) * pxns, (prevEnd - viewData.zvStart) * pxns, ty025, inactiveColor);
      }
      prev = -1;
    }

    if(activeFrameSet)
    {
      if(fend - fbegin > frameTarget)
      {
        QPointF p1 = wpos + QPointF((fbegin + frameTarget - viewData.zvStart) * pxns, 0);
        QPointF p2 =wpos + QPointF((fend - viewData.zvStart) * pxns, wh);
        painter->fillRect(QRectF(p1, p2) , getColor(0x224444FF));
      }
      if(fbegin >= viewData.zvStart && endPos != fbegin)
      {
        drawPolyLine(painter, dpos + QPointF((fbegin - viewData.zvStart) * pxns, 0), dpos + QPointF((fbegin - viewData.zvStart) * pxns, wh), 0x22FFFFFF);
      }
      if(fend <= viewData.zvEnd)
      {
        painter->setPen(getColor(0x22FFFFFF));
        painter->drawLine(dpos + QPointF((fend - viewData.zvStart) * pxns, 0), dpos + QPointF((fend - viewData.zvStart) * pxns, wh));
      }
      endPos = fend;
    }

    auto buf = getFrameText(fd, i, ftime);
    auto tx = getFontSize(buf).width();
    uint32_t color = (fd.name == 0 && i == 0) ? redColor : activeColor;

    if(fsz - 7 <= tx)
    {
      static char tmp[256];
      sprintf(tmp, "%s (%s)", tracy::RealToString(i), tracy::TimeToString(ftime));
      buf = tmp;
      tx = getFontSize(buf).width();
    }

    if(fsz - 7 <= tx)
    {
      buf = tracy::TimeToString(ftime);
      tx = getFontSize(buf).width();
    }

    if(fbegin >= viewData.zvStart)
    {
      drawPolyLine(painter, dpos + QPointF((fbegin - viewData.zvStart) * pxns + 2, 1), dpos + QPointF((fbegin - viewData.zvStart) * pxns + 2, ty - 1), color);
    }
    if(fend <= viewData.zvEnd)
    {
      drawPolyLine(painter, dpos + QPointF((fend - viewData.zvStart) * pxns - 2, 1), dpos + QPointF((fend - viewData.zvStart) * pxns - 2, ty - 1), color);
    }

    if(fsz - 7 > tx)
    {
      const auto f0 = (fbegin - viewData.zvStart) * pxns + 2;
      const auto f1 = (fend - viewData.zvStart) * pxns - 2;
      const auto x0 = f0 + 1;
      const auto x1 = f1 - 1;
      const auto te = x1 - tx;

      auto tpos = (x0 + te) / 2;
      if(tpos < 0)
      {
        tpos = std::min(std::min(0., te - tpos), te);
      }
      else if(tpos > w - tx)
      {
        tpos = std::max(double(w - tx), x0);
      }
      tpos = round(tpos);

      drawPolyLine(painter, dpos + QPointF(std::max(-10.0, f0), ty05), dpos + QPointF(tpos, ty05), color);
      drawPolyLine(painter, dpos + QPointF(std::max(-10.0, tpos + tx + 1), ty05), dpos + QPointF(std::min(w + 20.0, f1), ty05), color);
      painter->setPen(getColor(color));
      painter->drawText(wpos + QPointF(tpos, getFontSize(buf).height()), buf);
    }
    else
    {
      drawPolyLine(painter, dpos + QPointF(std::max(-10.0, (fbegin - viewData.zvStart) * pxns + 2), ty05), dpos + QPointF(std::min(w + 20.0, (fend - viewData.zvStart) * pxns - 2), ty05), color);
    }
    i++;
  }
}

void TimelineView::drawZonelist(const TimelineContext& ctx, const std::vector<tracy::TimelineDraw>& drawList,
  int _offset, uint64_t tid, QPainter* painter) {
  const auto w = ctx.w;
  const auto& wpos = ctx.wpos;
  const auto dpos = wpos + QPointF(0.5f, 0.5f);
  const auto ty = ctx.ty;
  const auto ostep = ty + 1;
  const auto yMin = ctx.yMin;
  const auto yMax = ctx.yMax;
  const auto pxns = ctx.pxns;
  const auto hover = ctx.hover;
  const auto vStart = ctx.vStart;

  for(auto& v : drawList) {
    const auto offset = _offset + ostep * v.depth;
    const auto yPos = wpos.y() + offset;

    if(yPos > yMax || yPos + ostep < yMin) {
      continue;
    }

    switch(v.type) {
      case tracy::TimelineDrawType::Folded: {
        auto& ev = *(const tracy::ZoneEvent*)v.ev.get();
        const auto color = v.inheritedColor ? v.inheritedColor : (viewData.dynamicColors == 2 ? 0xFF666666 : getThreadColor(tid, v.depth, viewData.dynamicColors));
        const auto rend = v.rend.Val();
        const auto px0 = (ev.Start() - vStart) * pxns;
        const auto px1 = (rend - vStart) * pxns;
        QPointF p1 = wpos + QPointF(std::max(px0, -10.0), offset);
        QPointF p2 = QPointF(std::min(std::max(px1 - px0, double(MinVisSize)), double(w + 10)), ty);
        painter->fillRect(p1.x(), p1.y(), p2.x(), p2.y(), getColor(color));
        if (hover) {

        }
        const auto tmp = tracy::RealToString(v.num);
        const auto tsz = getFontSize(tmp);
        if (tsz.width() < px1 - px0) {
          const auto x = px0 + (px1 - px0 - tsz.width()) / 2;
          drawTextContrast(painter, wpos + QPoint(x, offset), 0xFF4488DD, tmp);
        }
        break;
      }
      case tracy::TimelineDrawType::Zone: {
        auto& ev = *(const tracy::ZoneEvent*)v.ev.get();
        const auto end = worker.GetZoneEnd(ev);
        const auto zsz = std::max((end - ev.Start()) * pxns, pxns * 0.5);
        const auto zoneColor = getZoneColorData(ev, tid, v.depth, v.inheritedColor);
        const char* zoneName = worker.GetZoneName(ev);

        auto tsz = getFontSize(zoneName);
        if(viewData.shortenName == ShortenName::Always ||
          ((viewData.shortenName == ShortenName::NoSpace || viewData.shortenName == ShortenName::NoSpaceAndNormalize)
            && tsz.width() > zsz))
        {
          zoneName = shortenZoneName(viewData.shortenName, zoneName, tsz, zsz);
        }

        const auto pr0 = (ev.Start() - viewData.zvStart) * pxns;
        const auto pr1 = (end - viewData.zvStart) * pxns;
        const auto px0 = std::max(pr0, -10.0);
        const auto px1 = std::max({ std::min(pr1, double(w + 10)), px0 + pxns * 0.5, px0 + MinVisSize });
        painter->fillRect(px0 + wpos.x(), offset + wpos.y(), px1 - px0, tsz.height(), getColor(zoneColor.color));
        if (zoneColor.highlight) {
          // TODO hight light
        }
        else {
          const auto darkColor = tracy::DarkenColor(zoneColor.color);
          drawPolyLine(painter, dpos + QPointF(px0, offset + tsz.height()),
            dpos + QPointF(px0, offset),
            dpos + QPointF(px1 - 1, offset), zoneColor.accentColor, zoneColor.thickness);
          drawPolyLine(painter, dpos + QPointF(px0, offset + tsz.height()),
            dpos + QPointF(px1 - 1, offset + tsz.height()),
            dpos + QPointF(px1 - 1, offset),
            darkColor, zoneColor.thickness);
        }
        if (tsz.width() < zsz) {
          const auto x = (ev.Start() - viewData.zvStart) * pxns + ((end - ev.Start()) * pxns - tsz.width()) / 2;
          if(x < 0 || x > w - tsz.width()) {
            drawTextContrast(painter, wpos + QPointF(std::max(std::max(0., px0), std::min(double(w - tsz.width()), x)), offset),
              0xFFFFFFFF, zoneName);
          }
          else if(ev.Start() == ev.End()) {
            drawTextContrast(painter, wpos + QPointF(px0 + (px1 - px0 - tsz.width()) * 0.5, offset), 0xFFFFFFFF, zoneName);
          }
          else {
            drawTextContrast(painter, wpos + QPointF(x, offset), 0xFFFFFFFF, zoneName);
          }
        }
        else {
          QPointF topleft = wpos + QPoint(px0, offset - 1);
          QPointF bottomright = wpos + QPoint(px1 - px0, tsz.height() + 1);
          painter->setClipRect(topleft.x(), topleft.y(), bottomright.x(), bottomright.y());
          drawTextContrast(painter, wpos + QPointF(std::max(int64_t(0), ev.Start() - viewData.zvStart) * pxns, offset), 0xFFFFFFFF, zoneName);
          painter->setClipRect(0, 0, 0, 0, Qt::NoClip);
        }
        break;
      }
      default:
        assert(false);
        break;
    }
  }
}

void TimelineView::drawThread(const TimelineContext& context, const tracy::ThreadData& thread,
      const std::vector<tracy::TimelineDraw>& draws, int& offset, int depth, QPainter* painter) {
  
  const auto& wpos = context.wpos;
  const auto ty = context.ty;
  const auto ostep = ty + 1;
  const auto yMin = context.yMin;
  const auto yMax = context.yMax;

  // TODO draw samles and switch

  const auto yPos = wpos.y() + offset;
  if(!draws.empty() && yPos <= yMax && yPos + ostep * depth >= yMin)
  {
    drawZonelist(context, draws, offset, thread.id, painter);
  }
  offset += ostep * depth;
}

void TimelineView::drawTimeline(QPainter* painter) {
  auto timespan = viewData.zvEnd - viewData.zvStart;
  const auto width = this->width();
  auto pxns = width / double(timespan);

  auto yMin = 0;
  const auto yMax = this->height();
  const auto timeBegin = worker.GetFirstTime();
  const auto timeEnd = worker.GetLastTime();
  QPoint topLeft = QPoint(0, 0);
  QPoint bottomRight = QPoint(this->width(), this->height());
  if (timeBegin > viewData.zvStart) {
    topLeft.setX((timeBegin - viewData.zvStart) * pxns);
  }
  if (timeEnd < viewData.zvEnd) {
    topLeft.setX((timeEnd - viewData.zvStart) * pxns);
  }
  painter->fillRect(QRectF(topLeft, bottomRight), getColor(0x44000000));

  timelineController.begin();
  if (worker.AreFramesUsed()) {
    auto& frames = worker.GetFrames();
    for (auto frameData : frames) {
      if (vis(frameData)) {
        drawTimelineFrames(painter, *frameData, yMin);
      }
    }
  }


  if (viewData.drawGpuZones) {
    for (auto& v : worker.GetGpuData()) {
      // TODO Add GPUTimeline item
    }
  }
  if (viewData.drawCpuData && worker.HasContextSwitches()) {
    // TODO Add CpuTimeline item
  }
  if (viewData.drawZones) {
    const auto& threadData = worker.GetThreadData();
    if (threadData.size() != threadOrder.size()) {
      threadOrder.reserve(threadData.size());
      size_t numReinsert = threadReinsert.size();
      size_t numNew = threadData.size() - threadOrder.size() - numReinsert;
      for(size_t i = 0; i < numReinsert + numNew; i++)
      {
        const tracy::ThreadData *td = i < numReinsert ? threadReinsert[i] : threadData[threadOrder.size()];
        auto it = std::find_if(threadOrder.begin(), threadOrder.end(), [td](const auto t) {
          return td->groupHint < t->groupHint;
        });
        threadOrder.insert(it, td);
      }
      threadReinsert.clear();
    }
    for (const auto& v : threadOrder) {
      timelineController.addItem<TimelineItemThread>(v);
    }
  }

  if (viewData.drawPlots) {
    for (const auto& v : worker.GetPlots()) {
      // TODO Add plotTimeline item
    }
  }

  timelineController.end(pxns, QPoint(0, yMin), true, yMin, yMax, painter);
}

void TimelineView::zoomToRange(int64_t start, int64_t end, bool pause) {
  if( start == end )
  {
    end = start + 1;
  }

  if( pause )
  {
    viewMode = ViewMode::Paused;
  }
  hightlightZoom.active = false;
  zoomAnim.active = true;
  if (viewMode == ViewMode::LastRange) {
    const auto rangeCurr = viewData.zvEnd - viewData.zvStart;
    const auto rangeDest = end - start;
    zoomAnim.start0 = viewData.zvStart;
    zoomAnim.start1 = viewData.zvStart - ( rangeDest - rangeCurr );
    zoomAnim.end0 = viewData.zvEnd;
    zoomAnim.end1 = viewData.zvEnd;
  }
  else {
    zoomAnim.start0 = viewData.zvStart;
    zoomAnim.start1 = start;
    zoomAnim.end0 = viewData.zvEnd;
    zoomAnim.end1 = end;
  }
  zoomAnim.progress = 0;
}

void TimelineView::resizeEvent(QResizeEvent* event){
  update();
  QWidget::resizeEvent(event);
}


void TimelineView::paintEvent(QPaintEvent* event) {
  TestTime a("paint");
  auto painter = QPainter(this);
  drawTimeline(&painter);
  QWidget::paintEvent(event);
}

void TimelineView::mouseMoveEvent(QMouseEvent* event) {
  if (moveData.isDragging) {
    viewMode = ViewMode::Paused;
    const auto timespan = viewData.zvEnd - viewData.zvStart;
    zoomAnim.active = false;
    const auto w = width();
    const auto nspx = double(timespan) / w;
    auto delta = event->position().toPoint() - moveData.pos;
    moveData.pos = event->position().toPoint();
    auto yDelta = delta.y();
    const auto dpx = int64_t((delta.x() * nspx) + (moveData.hwheelDelta * nspx));
    if (dpx != 0) {
      viewData.zvStart -= dpx;
      viewData.zvEnd -= dpx;
      if( viewData.zvStart < -1000ll * 1000 * 1000 * 60 * 60 * 24 * 5 )
      {
        const auto range = viewData.zvEnd - viewData.zvStart;
        viewData.zvStart = -1000ll * 1000 * 1000 * 60 * 60 * 24 * 5;
        viewData.zvEnd = viewData.zvStart + range;
      }
      else if( viewData.zvEnd > 1000ll * 1000 * 1000 * 60 * 60 * 24 * 5 )
      {
        const auto range = viewData.zvEnd - viewData.zvStart;
        viewData.zvEnd = 1000ll * 1000 * 1000 * 60 * 60 * 24 * 5;
        viewData.zvStart = viewData.zvEnd - range;
      }
    }
    event->accept();
    update();
  }
  else {
    QWidget::mouseMoveEvent(event);
  }
}

void TimelineView::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::RightButton) {
    moveData.isDragging = true;
    moveData.pos = event->position().toPoint();
    event->accept();
  }
  else {
    QWidget::mousePressEvent(event);
  }
}

void TimelineView::mouseReleaseEvent(QMouseEvent* event)  {
  if (event->button() == Qt::RightButton) {
    moveData.isDragging = false;
    event->accept();
  }
  else {
    QWidget::mouseReleaseEvent(event);
  }
}

void TimelineView::wheelEvent(QWheelEvent* event) {
  if (viewMode == ViewMode::LastFrames) viewMode = ViewMode::LastRange;
  const auto mouse = mapFromGlobal(QCursor().pos()) - frameGeometry().topLeft();
  const auto p = double(mouse.x()) / width();
  int64_t t0, t1;
  if (zoomAnim.active) {
    t0 = zoomAnim.start1;
    t1 = zoomAnim.end1;
  }
  else {
    t0 = viewData.zvStart;
    t1 = viewData.zvEnd;
  }
  const auto zoomSpan = t1 - t0;
  const auto p1 = zoomSpan * p;
  const auto p2 = zoomSpan - p1;

  double mod = 0.05;
  auto wheel = event->angleDelta().y();
  if (wheel > 0) {
    t0 += int64_t(p1 * mod);
    t1 -= int64_t(p2 * mod);
  }
  else if( wheel < 0 && zoomSpan < 1000ll * 1000 * 1000 * 60 * 60 ) {
    t0 -= std::max( int64_t( 1 ), int64_t( p1 * mod ) );
    t1 += std::max( int64_t( 1 ), int64_t( p2 * mod ) );
  }
  zoomToRange(t0, t1, !worker.IsConnected() || viewMode == ViewMode::Paused);
  viewData.zvStart = int64_t( zoomAnim.start0 + zoomAnim.start1 - zoomAnim.start0);
  viewData.zvEnd = int64_t( zoomAnim.end0 + zoomAnim.end1 - zoomAnim.end0);
  update();
  event->accept();
}