#pragma once

#include "stdafx.h"

#include "DebugUtilsMessenger.h"
#include "QueueFamilyIndices.h"

namespace zvk
{

struct HelloTriangleApplication
{
    HelloTriangleApplication();

    void Run();

  private:
    GLFWwindow *m_window;

    std::unique_ptr<DebugUtilsMessenger> m_standaloneDebugMessenger;
    vk::UniqueInstance m_instance;
    std::unique_ptr<DebugUtilsMessenger> m_instanceDebugMessenger;
    std::unique_ptr<QueueFamilyIndices> m_familyIndices;
    vk::PhysicalDevice m_physicalDevice;
    vk::UniqueDevice m_device;
    vk::Queue m_graphicsQueue;

    void InitWindow();
    void CreateInstance();
    void SetupDebugMessenger();
    bool IsDeviceSuitable(vk::PhysicalDevice device);
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void InitVulkan();
    void MainLoop();
    void Cleanup();
};

} // namespace zvk
