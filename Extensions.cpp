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

} // namespace zvk
