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

#pragma once

#include <tgfx/core/Data.h>
#include "SerializationUtils.h"

namespace tgfx {
class LayerStyle;
class LayerStyleSerialization {
 public:
  static std::shared_ptr<Data> Serialize(const LayerStyle* layerStyle, SerializeUtils::Map* map);

 private:
  static void SerializeBasicLayerStyleImpl(flexbuffers::Builder& fbb, const LayerStyle* layerStyle);

  static void SerializeBackGroundBlurStyleImpl(flexbuffers::Builder& fbb,
                                               const LayerStyle* layerStyle);

  static void SerializeDropShadowStyleImpl(flexbuffers::Builder& fbb, const LayerStyle* layerStyle,
                                           SerializeUtils::Map* map);

  static void SerializeInnerShadowStyleImpl(flexbuffers::Builder& fbb, const LayerStyle* layerStyle,
                                            SerializeUtils::Map* map);
};
}  // namespace tgfx
