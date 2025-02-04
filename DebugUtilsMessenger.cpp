#include "DebugUtilsMessenger.h"

#include "Strings.h"

namespace zvk
{

static VKAPI_ATTR VkBool32 VKAPI_CALL
DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
{
    DebugUtilsMessenger *messenger = static_cast<DebugUtilsMessenger *>(pUserData);
    return messenger->DebugCallback(messageSeverity, messageType, pCallbackData);
}

DebugUtilsMessenger::DebugUtilsMessenger()
{
}

DebugUtilsMessenger::DebugUtilsMessenger(vk::Instance instance)
{
    m_instanceDebugMessenger = instance.createDebugUtilsMessengerEXTUnique(CreateInfo());
}

vk::DebugUtilsMessengerCreateInfoEXT DebugUtilsMessenger::CreateInfo()
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
DebugUtilsMessenger::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                   VkDebugUtilsMessageTypeFlagsEXT messageType,
                                   const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData)
{
    std::cerr << "*** " << pCallbackData->pMessage << "\n";

#ifdef _WIN32
    std::wstring message = UTF8ToWideChar(pCallbackData->pMessage);
    OutputDebugString(L"*** ");
    OutputDebugString(message.c_str());
    OutputDebugString(L"\n");
#endif

    // stop the application
    return VK_TRUE;
}

} // namespace zvk