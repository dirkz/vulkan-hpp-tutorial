#pragma once

#include "stdafx.h"

namespace zvk
{

struct DebugUtilsMessenger
{
    /// <summary>
    /// Create a stand-alone debug utils messenger suitable during instance creation
    /// and its destruction.
    /// </summary>
    DebugUtilsMessenger();

    /// <summary>
    /// Create a debug utils messenger tied to an (already created) instance.
    /// </summary>
    /// <param name="instance">The instance to receive messages from</param>
    DebugUtilsMessenger(const vk::Instance instance);

    /// <summary>
    /// Shared method for creating a <code>DebugUtilsMessengerCreateInfoEXT</code>.
    /// </summary>
    /// <returns></returns>
    vk::DebugUtilsMessengerCreateInfoEXT CreateInfo();

    VKAPI_ATTR VkBool32 VKAPI_CALL
    DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData);

  private:
    vk::UniqueDebugUtilsMessengerEXT m_instanceDebugMessenger;
};

} // namespace zvk
