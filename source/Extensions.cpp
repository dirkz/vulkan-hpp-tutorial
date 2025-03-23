#include "Extensions.h"

#include "Constants.h"

using namespace std;

namespace zvk
{

static vector<const char *> RequiredWindowExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    vector<const char *> extensions(glfwExtensionCount);

    for (uint32_t i = 0; i < glfwExtensionCount; ++i)
    {
        extensions[i] = glfwExtensions[i];
    }

    return extensions;
}

std::vector<const char *> RequiredInstanceExtensions()
{
    vector<const char *> extensions = RequiredWindowExtensions();

    if (EnableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

std::vector<const char *> RequiredDeviceExtensions()
{
    // Note: Extension names can be found in vulkan_core.h
    static const vector<const char *> requiredDeviceExtensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
        VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME};

    return requiredDeviceExtensions;
}

bool CheckDeviceExtensionSupport(vk::PhysicalDevice device)
{
    vector<vk::ExtensionProperties> extensionProperties =
        device.enumerateDeviceExtensionProperties();

    set<string> extensionNames{};
    for (const vk::ExtensionProperties &properties : extensionProperties)
    {
        extensionNames.insert(properties.extensionName);
    }

    for (const char *deviceExtension : RequiredDeviceExtensions())
    {
        auto it = find(extensionNames.begin(), extensionNames.end(), deviceExtension);
        if (it == extensionNames.end())
        {
            return false;
        }
    }

    return true;
}

} // namespace zvk
