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

#include "RuntimeDrawTask.h"
#include "gpu/Pipeline.h"
#include "gpu/ProgramCache.h"
#include "gpu/Quad.h"
#include "gpu/RuntimeProgramCreator.h"
#include "gpu/RuntimeProgramWrapper.h"
#include "gpu/processors/DefaultGeometryProcessor.h"
#include "gpu/processors/TextureEffect.h"

namespace tgfx {
RuntimeDrawTask::RuntimeDrawTask(std::shared_ptr<RenderTargetProxy> target,
                                 std::vector<std::shared_ptr<TextureProxy>> inputs,
                                 std::shared_ptr<RuntimeEffect> effect, const Point& offset)
    : RenderTask(std::move(target)), inputs(std::move(inputs)), effect(std::move(effect)),
      offset(offset) {
}

bool RuntimeDrawTask::execute(RenderPass* renderPass) {
  std::vector<std::shared_ptr<Texture>> inputTextures = {};
  inputTextures.reserve(inputs.size());
  for (size_t i = 0; i < inputs.size(); i++) {
    std::shared_ptr<Texture> texture;
    if (inputs[i] != nullptr) {
      texture = GetFlatTexture(renderPass, inputs[i]);
    }
    if (texture == nullptr) {
      LOGE("RuntimeDrawTask::execute() Failed to get the input %d texture!", i);
      return false;
    }
    inputTextures.push_back(texture);
  }

  auto renderTarget = renderTargetProxy->getRenderTarget();
  if (renderTarget == nullptr) {
    LOGE("RuntimeDrawTask::execute() Failed to get the render target!");
    return false;
  }
  auto context = renderPass->getContext();
  RuntimeProgramCreator programCreator(effect);
  auto program = context->programCache()->getProgram(&programCreator);
  if (program == nullptr) {
    LOGE("RuntimeDrawTask::execute() Failed to create the runtime program!");
    return false;
  }
  std::vector<BackendTexture> backendTextures = {};
  backendTextures.reserve(inputTextures.size());
  for (auto& texture : inputTextures) {
    backendTextures.push_back(texture->getBackendTexture());
  }
  return effect->onDraw(RuntimeProgramWrapper::Unwrap(program), backendTextures,
                        renderTarget->getBackendRenderTarget(), offset);
}

std::shared_ptr<Texture> RuntimeDrawTask::GetFlatTexture(
    RenderPass* renderPass, std::shared_ptr<TextureProxy> textureProxy) {
  auto texture = textureProxy->getTexture();
  if (texture == nullptr) {
    return nullptr;
  }
  if (!texture->isYUV() && texture->getSampler()->type() == SamplerType::TwoD &&
      texture->origin() == ImageOrigin::TopLeft) {
    return texture;
  }
  auto context = renderPass->getContext();
  auto renderTargetProxy = RenderTargetProxy::MakeFallback(
      context, texture->width(), texture->height(), texture->isAlphaOnly(), 1,
      texture->hasMipmaps(), ImageOrigin::TopLeft, BackingFit::Exact);
  if (renderTargetProxy == nullptr) {
    LOGE("RuntimeDrawTask::getFlatTexture() Failed to create the render target!");
    return nullptr;
  }
  auto renderTarget = renderTargetProxy->getRenderTarget();
  if (!renderPass->begin(renderTarget)) {
    LOGE("RuntimeDrawTask::getFlatTexture() Failed to initialize the render pass!");
    return nullptr;
  }
  auto colorProcessor = TextureEffect::Make(std::move(textureProxy));
  if (colorProcessor == nullptr) {
    LOGE("RuntimeDrawTask::getFlatTexture() Failed to create the color processor!");
    return nullptr;
  }
  auto drawingBuffer = renderPass->getContext()->drawingBuffer();
  auto geometryProcessor = DefaultGeometryProcessor::Make(
      drawingBuffer, {}, renderTarget->width(), renderTarget->height(), AAType::None, {}, {});
  auto format = renderPass->renderTarget()->format();
  auto caps = renderPass->getContext()->caps();
  const auto& swizzle = caps->getWriteSwizzle(format);
  std::vector<PlacementPtr<FragmentProcessor>> fragmentProcessors = {};
  fragmentProcessors.emplace_back(std::move(colorProcessor));
  auto pipeline =
      std::make_unique<Pipeline>(std::move(geometryProcessor), std::move(fragmentProcessors), 1,
                                 nullptr, BlendMode::Src, &swizzle);
  auto quad = Quad::MakeFrom(Rect::MakeWH(renderTarget->width(), renderTarget->height()));
  auto vertexData = quad.toTriangleStrips();
  renderPass->bindProgramAndScissorClip(pipeline.get(), {});
  renderPass->bindBuffers(nullptr, vertexData);
  renderPass->draw(PrimitiveType::TriangleStrip, 0, 4);
  renderPass->end();
  return renderTarget->asTexture();
}

}  // namespace tgfx
