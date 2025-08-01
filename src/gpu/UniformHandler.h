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

#include "SamplerHandle.h"
#include "ShaderVar.h"
#include "Swizzle.h"
#include "gpu/TextureSampler.h"

namespace tgfx {
class ProgramBuilder;

class UniformHandler {
 public:
  virtual ~UniformHandler() = default;

  /**
   * Adds a uniform variable to the current program, that has visibility in one or more shaders.
   * visibility is a bitfield of ShaderFlag values indicating from which shaders the uniform should
   * be accessible. At least one bit must be set. The actual uniform name will be mangled. Returns
   * the final uniform name.
   */
  std::string addUniform(ShaderFlags visibility, SLType type, const std::string& name) {
    return internalAddUniform(visibility, type, name);
  }

  /**
   * Adds a sampler to the current program.
   */
  SamplerHandle addSampler(const TextureSampler* sampler, const std::string& name);

 protected:
  explicit UniformHandler(ProgramBuilder* program) : programBuilder(program) {
  }

  // This is not owned by the class
  ProgramBuilder* programBuilder;

 private:
  virtual const ShaderVar& samplerVariable(SamplerHandle samplerHandle) const = 0;

  virtual const Swizzle& samplerSwizzle(SamplerHandle samplerHandle) const = 0;

  virtual std::string internalAddUniform(ShaderFlags visibility, SLType type,
                                         const std::string& name) = 0;

  virtual SamplerHandle internalAddSampler(const TextureSampler* sampler,
                                           const std::string& name) = 0;

  virtual std::string getUniformDeclarations(ShaderFlags visibility) const = 0;

  friend class ProgramBuilder;
};
}  // namespace tgfx
