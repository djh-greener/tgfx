/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Tencent is pleased to support the open source community by making tgfx available.
//
//  Copyright (C) 2023 Tencent. All rights reserved.
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

#include <emscripten/val.h>
#include "tgfx/platform/ImageReader.h"

namespace tgfx {
/**
 * The VideoElementReader class allows direct access to image buffers rendered into a
 * HTMLVideoElement on the web platform.
 */
class VideoElementReader : public ImageReader {
 public:
  /**
   * Creates a new VideoElementReader from the specified HTMLVideoElement object and the video size.
   * Returns nullptr if the video is null or the buffer size is zero.
   */
  static std::shared_ptr<VideoElementReader> MakeFrom(emscripten::val video, int width, int height);

  /**
   * Acquires the next ImageBuffer from the VideoElementReader after a new image frame is about to
   * be rendered into the associated HTMLVideoElement. The returned ImageBuffer will call the
   * promise.await() method before generating textures. Note that the previously returned image
   * buffers will immediately expire after the newly created ImageBuffer is drawn.
   */
  std::shared_ptr<ImageBuffer> acquireNextBuffer(emscripten::val promise);

 private:
  explicit VideoElementReader(std::shared_ptr<ImageStream> stream);
};
}  // namespace tgfx
