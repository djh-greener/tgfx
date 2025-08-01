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

#include "gpu/Texture.h"

namespace tgfx {
/**
 * RenderTarget represents a 2D buffer of pixels that can be rendered to.
 */
class RenderTarget {
 public:
  virtual ~RenderTarget() = default;
  /**
   * Wraps a backend renderTarget into RenderTarget. The caller must ensure the backend renderTarget
   * is valid for the lifetime of the returned RenderTarget. Returns nullptr if the context is
   * nullptr or the backend renderTarget is invalid.
   */
  static std::shared_ptr<RenderTarget> MakeFrom(Context* context,
                                                const BackendRenderTarget& backendRenderTarget,
                                                ImageOrigin origin = ImageOrigin::TopLeft);

  /**
   * Creates a new RenderTarget with an existing backend texture. If adopted is true, the
   * backend texture will be destroyed at a later point after the proxy is released.
   */
  static std::shared_ptr<RenderTarget> MakeFrom(Context* context,
                                                const BackendTexture& backendTexture,
                                                int sampleCount = 1,
                                                ImageOrigin origin = ImageOrigin::TopLeft,
                                                bool adopted = false);

  /**
   * Creates a new RenderTarget with an existing HardwareBuffer and sample count.
   */
  static std::shared_ptr<RenderTarget> MakeFrom(Context* context, HardwareBufferRef hardwareBuffer,
                                                int sampleCount = 1);

  /**
   * Creates a new RenderTarget instance with specified context, with, height, format, sample count,
   * mipmap state and origin.
   */
  static std::shared_ptr<RenderTarget> Make(Context* context, int width, int height,
                                            PixelFormat format = PixelFormat::RGBA_8888,
                                            int sampleCount = 1, bool mipmapped = false,
                                            ImageOrigin origin = ImageOrigin::TopLeft);

  /**
   * Returns the context associated with the RenderTarget.
   */
  virtual Context* getContext() const = 0;

  /**
   * Returns the width of the render target.
   */
  virtual int width() const = 0;

  /**
   * Returns the height of the render target.
   */
  virtual int height() const = 0;

  /**
   * Returns the origin of the render target, either ImageOrigin::TopLeft or
   * ImageOrigin::BottomLeft.
   */
  virtual ImageOrigin origin() const = 0;

  /**
   * Returns the sample count of the render target.
   */
  virtual int sampleCount() const = 0;

  /**
   * Returns the pixel format of the render target.
   */
  virtual PixelFormat format() const = 0;

  /**
   * Returns true if the render target is externally owned.
   */
  virtual bool externallyOwned() const = 0;

  /**
   * Returns a reference to the underlying texture representation of this render target, may be
   * nullptr.
   */
  virtual std::shared_ptr<Texture> asTexture() const {
    return nullptr;
  }

  /**
   * Retrieves the backend render target.
   */
  virtual BackendRenderTarget getBackendRenderTarget() const = 0;

  /**
   * Copies a rect of pixels to dstPixels with specified color type, alpha type and row bytes. Copy
   * starts at (srcX, srcY), and does not exceed Surface (width(), height()). Pixels are copied
   * only if pixel conversion is possible. Returns true if pixels are copied to dstPixels.
   */
  virtual bool readPixels(const ImageInfo& dstInfo, void* dstPixels, int srcX = 0,
                          int srcY = 0) const = 0;
};
}  // namespace tgfx
