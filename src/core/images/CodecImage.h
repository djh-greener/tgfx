/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Tencent is pleased to support the open source community by making tgfx available.
//
//  Copyright (C) 2024 Tencent. All rights reserved.
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

#include <memory>
#include "core/images/GeneratorImage.h"
#include "tgfx/core/Image.h"
#include "tgfx/core/ImageCodec.h"

namespace tgfx {

class CodecImage : public GeneratorImage {
 public:
  static std::shared_ptr<Image> MakeFrom(const std::shared_ptr<ImageCodec>& codec);

  ~CodecImage() override = default;

  std::shared_ptr<ImageCodec> codec() const;

 protected:
  Type type() const override {
    return Type::Codec;
  }

 private:
  explicit CodecImage(const std::shared_ptr<ImageCodec>& codec);
};

}  // namespace tgfx