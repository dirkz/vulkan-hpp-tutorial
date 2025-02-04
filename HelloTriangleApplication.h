#pragma once

#include "stdafx.h"

#include "DebugUtilsMessenger.h"

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
    vk::PhysicalDevice m_physicalDevice;

    void InitWindow();
    void CreateInstance();
    void SetupDebugMessenger();
    bool IsDeviceSuitable(vk::PhysicalDevice device);
    void PickPhysicalDevice();
    void InitVulkan();
    void MainLoop();
    void Cleanup();
};

} // namespace zvk
