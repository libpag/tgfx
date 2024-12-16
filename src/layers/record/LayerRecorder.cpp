#include "tgfx/layers/record/LayerRecorder.h"
#include <layers/record/LayerCmd.h>
#include <tgfx/layers/Layer.h>
#include <tgfx/layers/record/Recorder.h>

namespace tgfx {

void LayerRecorder::SetDefaultAllowsEdgeAntialiasing(bool value) {
  Recorder::Record(std::make_unique<CmdSetDefaultAllowsEdgeAntialiasing>(value));
}

void LayerRecorder::SetDefaultAllowsGroupOpacity(bool value) {
  Recorder::Record(std::make_unique<CmdSetDefaultAllowsGroupOpacity>(value));
}

void LayerRecorder::MakeLayer(Layer* layer) {
  Recorder::Record(std::make_unique<CmdMakeLayer>(layer->_uuid));
}

void LayerRecorder::setName(Layer* layer, const std::string& value) {
  Recorder::Record(std::make_unique<CmdSetName>(layer->_uuid, value));
}

void LayerRecorder::setAlpha(Layer* layer, float value) {
  Recorder::Record(std::make_unique<CmdSetAlpha>(layer->_uuid, value));
}

void LayerRecorder::setBlendMode(Layer* layer, BlendMode value) {
  Recorder::Record(std::make_unique<CmdSetBlendMode>(layer->_uuid, value));
}

void LayerRecorder::setPosition(Layer* layer, const Point& value) {
  Recorder::Record(std::make_unique<CmdSetPosition>(layer->_uuid, value));
}

void LayerRecorder::setMatrix(Layer* layer, const Matrix& value) {
  Recorder::Record(std::make_unique<CmdSetMatrix>(layer->_uuid, value));
}

void LayerRecorder::setVisible(Layer* layer, bool value) {
  Recorder::Record(std::make_unique<CmdSetVisible>(layer->_uuid, value));
}

void LayerRecorder::setShouldRasterize(Layer* layer, bool value) {
  Recorder::Record(std::make_unique<CmdSetShouldRasterize>(layer->_uuid, value));
}

void LayerRecorder::setRasterizationScale(Layer* layer, float value) {
  Recorder::Record(std::make_unique<CmdSetRasterizationScale>(layer->_uuid, value));
}

void LayerRecorder::setAllowsEdgeAntialiasing(Layer* layer, bool value) {
  Recorder::Record(std::make_unique<CmdSetAllowsEdgeAntialiasing>(layer->_uuid, value));
}

void LayerRecorder::setAllowsGroupOpacity(Layer* layer, bool value) {
  Recorder::Record(std::make_unique<CmdSetAllowsGroupOpacity>(layer->_uuid, value));
}

void LayerRecorder::setFilters(Layer* layer, const std::vector<std::shared_ptr<LayerFilter>>& layer_filters) {
  std::vector<int> filter_ids;
  for (const auto& filter : layer_filters) {
    filter_ids.emplace_back(filter->_uuid);
  }
  Recorder::Record(std::make_unique<CmdSetFilters>(layer->_uuid, filter_ids));
}

void LayerRecorder::setMask(Layer* layer, const std::shared_ptr<Layer>& mask) {
  Recorder::Record(std::make_unique<CmdSetMask>(layer->_uuid, mask->_uuid));
}

void LayerRecorder::setScrollRect(Layer* layer, const Rect& rect) {
  Recorder::Record(std::make_unique<CmdSetScrollRect>(layer->_uuid, rect));
}

void LayerRecorder::addChildAt(Layer* layer, const std::shared_ptr<Layer>& child, int index) {
  Recorder::Record(std::make_unique<CmdAddChildAt>(layer->_uuid, child->_uuid, index));
}

void LayerRecorder::removeChildAt(Layer* layer, int index) {
  Recorder::Record(std::make_unique<CmdRemoveChildAt>(layer->_uuid, index));
}

void LayerRecorder::removeChildren(Layer* layer, int beginIndex, int endIndex) {
  Recorder::Record(std::make_unique<CmdRemoveChildren>(layer->_uuid, beginIndex, endIndex));
}

void LayerRecorder::removeFromParent(Layer* layer) {
  Recorder::Record(std::make_unique<CmdRemoveFromParent>(layer->_uuid));
}

void LayerRecorder::setChildIndex(Layer* layer, const std::shared_ptr<Layer>& child, int index) {
  Recorder::Record(std::make_unique<CmdSetChildIndex>(layer->_uuid, child->_uuid, index));
}

void LayerRecorder::replaceChild(Layer* layer, const std::shared_ptr<Layer>& old_child, const std::shared_ptr<Layer>& new_child) {
  Recorder::Record(std::make_unique<CmdReplaceChild>(layer->_uuid, old_child->_uuid, new_child->_uuid));
}

}  // namespace tgfx