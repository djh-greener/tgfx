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

#include "NativeImageBuffer.h"
#include <android/bitmap.h>
#include "core/utils/Log.h"
#include "tgfx/platform/android/AndroidBitmap.h"

namespace tgfx {
std::shared_ptr<ImageBuffer> NativeImageBuffer::MakeFrom(JNIEnv* env, jobject bitmap) {
  auto info = AndroidBitmap::GetInfo(env, bitmap);
  if (info.isEmpty() ||
      (info.colorType() != ColorType::RGBA_8888 && info.colorType() != ColorType::ALPHA_8) ||
      info.alphaType() == AlphaType::Unpremultiplied) {
    return nullptr;
  }
  auto pixelBuffer = std::shared_ptr<NativeImageBuffer>(new NativeImageBuffer(info));
  pixelBuffer->bitmap = bitmap;
  return pixelBuffer;
}

std::shared_ptr<Texture> NativeImageBuffer::onMakeTexture(Context* context, bool mipmapped) const {
  JNIEnvironment environment;
  auto env = environment.current();
  if (env == nullptr) {
    return nullptr;
  }
  void* pixels = nullptr;
  if (AndroidBitmap_lockPixels(env, bitmap.get(), &pixels) != 0) {
    env->ExceptionClear();
    LOGE("NativeImageBuffer::onMakeTexture() Failed to lockPixels() from a Java Bitmap!");
    return nullptr;
  }
  std::shared_ptr<Texture> texture = nullptr;
  if (isAlphaOnly()) {
    texture = Texture::MakeAlpha(context, info.width(), info.height(), pixels, info.rowBytes(),
                                 mipmapped);
  } else {
    texture =
        Texture::MakeRGBA(context, info.width(), info.height(), pixels, info.rowBytes(), mipmapped);
  }
  AndroidBitmap_unlockPixels(env, bitmap.get());
  return texture;
}
}  // namespace tgfx
