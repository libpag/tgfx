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

#include <cstdint>
#include <memory>
#include "core/DrawContext.h"
#include "core/FillStyle.h"
#include "core/MCState.h"
#include "svg/xml/XMLWriter.h"
#include "tgfx/core/Path.h"
#include "tgfx/core/Rect.h"
#include "tgfx/core/Size.h"
#include "tgfx/gpu/Context.h"
#include "tgfx/svg/SVGParse.h"

namespace tgfx {

class ResourceStore;
class ElementWriter;

class SVGContext : public DrawContext {
 public:
  SVGContext(Context* GPUContext, const ISize& size, std::unique_ptr<XMLWriter> writer,
             uint32_t flags);
  ~SVGContext() override = default;

  void clear() override;

  void drawRect(const Rect&, const MCState&, const FillStyle&) override;

  void drawRRect(const RRect&, const MCState&, const FillStyle&) override;

  void drawPath(const Path&, const MCState&, const FillStyle&, const Stroke*) override;

  void drawImage(std::shared_ptr<Image> image, const SamplingOptions& sampling,
                 const MCState& state, const FillStyle& style) override;

  void drawImageRect(std::shared_ptr<Image> image, const Rect& rect,
                     const SamplingOptions& sampling, const MCState& state,
                     const FillStyle& style) override;

  void drawGlyphRunList(std::shared_ptr<GlyphRunList>, const MCState&, const FillStyle&,
                        const Stroke*) override;

  void drawPicture(std::shared_ptr<Picture>, const MCState&) override;

  void drawLayer(std::shared_ptr<Picture>, const MCState&, const FillStyle&,
                 std::shared_ptr<ImageFilter>) override {};

  void syncClipStack(const MCState& mc);

  XMLWriter* getWriter() const {
    return _writer.get();
  }

 private:
  /**
   * Determine if the paint requires us to reset the viewport.Currently, we do this whenever the
   * paint shader calls for a repeating image.
   */
  bool RequiresViewportReset(const FillStyle& fill);

  void drawColorGlyphs(const std::shared_ptr<GlyphRunList>& glyphRunList, const MCState& state,
                       const FillStyle& style);

  PathParse::PathEncoding pathEncoding() const {
    return PathParse::PathEncoding::Absolute;
  }

  ISize _size;
  Context* _context = nullptr;
  const std::unique_ptr<XMLWriter> _writer;
  const std::unique_ptr<ResourceStore> _resourceBucket;
  const uint32_t _flags;
  std::unique_ptr<ElementWriter> _rootElement;

  struct ClipRecord {
    const MCState* clip;
    std::unique_ptr<ElementWriter> element;
  };

  std::vector<ClipRecord> _clipStack;
  int _clipAttributeCount = 0;
};
}  // namespace tgfx