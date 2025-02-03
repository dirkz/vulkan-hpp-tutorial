#pragma once

#include "stdafx.h"

namespace zvk
{

struct DebugMessenger
{
    DebugMessenger(vk::Instance instance);

    VKAPI_ATTR VkBool32 VKAPI_CALL
    DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData);
};

} // namespace zvk
