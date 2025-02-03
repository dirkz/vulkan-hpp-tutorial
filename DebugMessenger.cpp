#include "DebugMessenger.h"

namespace zvk
{

static VKAPI_ATTR VkBool32 VKAPI_CALL
DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
    DebugMessenger *messenger = static_cast<DebugMessenger *>(pUserData);
    return messenger->DebugCallback(messageSeverity, messageType, pCallbackData);
}

DebugMessenger::DebugMessenger()
{
}

DebugMessenger::DebugMessenger(vk::Instance instance)
{
    m_instanceDebugMessenger = instance.createDebugUtilsMessengerEXTUnique(CreateInfo());
}

vk::DebugUtilsMessengerCreateInfoEXT DebugMessenger::CreateInfo()
{
    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags =
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning;
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags =
        vk::FlagTraits<vk::DebugUtilsMessageTypeFlagBitsEXT>::allFlags;

    vk::DebugUtilsMessengerCreateInfoEXT createInfo{
        {}, severityFlags, messageTypeFlags, zvk::DebugCallback, this};

    return createInfo;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
DebugMessenger::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                              VkDebugUtilsMessageTypeFlagsEXT messageType,
                              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

} // namespace zvk