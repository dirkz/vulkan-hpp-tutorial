#pragma once

#include "stdafx.h"

#include "DebugUtilsMessenger.h"
#include "QueueFamilyIndices.h"
#include "Window.h"

namespace zvk
{

struct HelloTriangleApplication
{
    void Run();

  private:
    std::unique_ptr<Window> m_window;

    std::unique_ptr<DebugUtilsMessenger> m_standaloneDebugMessenger;
    vk::UniqueInstance m_instance;
    std::unique_ptr<DebugUtilsMessenger> m_instanceDebugMessenger;
    vk::UniqueSurfaceKHR m_surface;
    vk::PhysicalDevice m_physicalDevice;
    vk::UniqueDevice m_device;
    vk::Queue m_graphicsQueue;
    vk::Queue m_presentQueue;

    void InitWindow();
    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    bool IsDeviceSuitable(vk::PhysicalDevice device) const;
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain();
    void InitVulkan();
    void MainLoop();
    void Cleanup();
};

} // namespace zvk
