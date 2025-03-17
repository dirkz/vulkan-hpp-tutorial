#pragma once

#include "stdafx.h"

namespace zvk
{

struct ShaderModule
{
    ShaderModule(const vk::Device device, const std::filesystem::path basePath,
                 const std::string filename);

    vk::ShaderModule Module();

  private:
    vk::UniqueShaderModule m_module;
};

} // namespace zvk
