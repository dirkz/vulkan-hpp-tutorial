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

bool CheckDeviceExtensionSupport(vk::PhysicalDevice device)
{
    static const vector<const char *> requiredDeviceExtensions{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    vector<vk::ExtensionProperties> extensionProperties =
        device.enumerateDeviceExtensionProperties();

    vector<const char *> extensionNames(extensionProperties.size());
    transform(extensionProperties.begin(), extensionProperties.end(), extensionNames.begin(),
              [](const vk::ExtensionProperties &properties) { return properties.extensionName; });

    return true;
}

} // namespace zvk
