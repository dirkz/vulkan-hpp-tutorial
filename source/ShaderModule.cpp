#include "ShaderModule.h"

namespace zvk
{

ShaderModule::ShaderModule(vk::Device device, std::filesystem::path basePath, std::string filename)
{
    std::filesystem::path filePath = basePath / filename;
    std::ifstream file{filePath, std::ios::ate | std::ios::binary};

    if (!file.is_open())
    {
        throw std::runtime_error{"can't open shader file"};
    }

    size_t fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    vk::ShaderModuleCreateInfo createInfo{
        {}, fileSize, reinterpret_cast<uint32_t *>(buffer.data())};

    m_module = device.createShaderModuleUnique(createInfo);
}

} // namespace zvk