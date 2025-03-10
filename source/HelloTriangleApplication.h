#pragma once

#include "stdafx.h"

#include "DebugUtilsMessenger.h"
#include "QueueFamilyIndices.h"
#include "SwapChain.h"
#include "Window.h"

namespace zvk
{

struct HelloTriangleApplication
{
    HelloTriangleApplication(std::filesystem::path shaderPath);

    void Run();

  private:
    std::filesystem::path m_shaderPath;

    std::unique_ptr<Window> m_window;

    std::unique_ptr<DebugUtilsMessenger> m_standaloneDebugMessenger;
    vk::UniqueInstance m_instance;
    std::unique_ptr<DebugUtilsMessenger> m_instanceDebugMessenger;

    vk::UniqueSurfaceKHR m_surface;

    vk::PhysicalDevice m_physicalDevice;
    vk::UniqueDevice m_device;
    std::unique_ptr<QueueFamilyIndices> m_familyIndices;

    std::unique_ptr<SwapChain> m_swapchain;

    vk::UniqueRenderPass m_renderPass;
    vk::UniquePipelineLayout m_pipelineLayout;
    vk::UniquePipeline m_graphicsPipeline;

    vk::UniqueCommandPool m_commandPool;
    vk::UniqueCommandBuffer m_commandBuffer;

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
    void CreateRenderPass();
    void CreateGraphicsPipeline();
    void CreateFrameBuffers();
    void CreateCommandPool();
    void CreateCommandBuffer();
    void RecordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex);

    void InitVulkan();
    void MainLoop();
    void DrawFrame();
    void Cleanup();
};

} // namespace zvk
