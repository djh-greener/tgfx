/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Tencent is pleased to support the open source community by making tgfx available.
//
//  Copyright (C) 2025 Tencent. All rights reserved.
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
#ifdef TGFX_USE_INSPECTOR
#include "SamplingOptionsSerialization.h"

namespace tgfx {

std::shared_ptr<Data> SamplingOptionsSerialization::Serialize(
    const SamplingOptions* samplingOptions) {
  DEBUG_ASSERT(samplingOptions != nullptr)
  flexbuffers::Builder fbb;
  size_t startMap;
  size_t contentMap;
  SerializeUtils::SerializeBegin(fbb, "LayerAttribute", startMap, contentMap);
  SerializeSamplingOptionsImpl(fbb, samplingOptions);
  SerializeUtils::SerializeEnd(fbb, startMap, contentMap);
  return Data::MakeWithCopy(fbb.GetBuffer().data(), fbb.GetBuffer().size());
}

void SamplingOptionsSerialization::SerializeSamplingOptionsImpl(
    flexbuffers::Builder& fbb, const SamplingOptions* samplingOptions) {
  SerializeUtils::SetFlexBufferMap(fbb, "filterMode",
                                   SerializeUtils::FilterModeToString(samplingOptions->filterMode));
  SerializeUtils::SetFlexBufferMap(fbb, "mipmapMode",
                                   SerializeUtils::MipmapModeToString(samplingOptions->mipmapMode));
}
}  // namespace tgfx
#endif