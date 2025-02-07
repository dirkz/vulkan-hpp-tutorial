#pragma once

#include "stdafx.h"

namespace zvk
{

struct ShaderModule
{
    ShaderModule(vk::Device device, std::filesystem::path basePath, std::string filename);

    vk::ShaderModule Module();

  private:
    vk::UniqueShaderModule m_module;
};

} // namespace zvk
