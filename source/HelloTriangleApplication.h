#pragma once

#include "stdafx.h"

#include "Constants.h"
#include "DebugUtilsMessenger.h"
#include "FrameData.h"
#include "QueueFamilyIndices.h"
#include "SwapChain.h"
#include "UniformBufferObject.h"
#include "UploadQueue.h"
#include "Vertex.h"
#include "Vma.h"
#include "VmaBuffer.h"
#include "Window.h"

namespace zvk
{

struct HelloTriangleApplication : public Window::Callback
{
    HelloTriangleApplication(std::filesystem::path shaderPath, std::filesystem::path texturePath);

    void Run();

    void Resize(int width, int height) override;

  private:
    std::filesystem::path m_shaderPath;
    std::filesystem::path m_texturePath;

    std::unique_ptr<Window> m_window;

    std::unique_ptr<DebugUtilsMessenger> m_standaloneDebugMessenger;
    vk::UniqueInstance m_instance;
    std::unique_ptr<DebugUtilsMessenger> m_instanceDebugMessenger;

    vk::UniqueSurfaceKHR m_surface;

    vk::PhysicalDevice m_physicalDevice;
    vk::UniqueDevice m_device;
    std::unique_ptr<QueueFamilyIndices> m_familyIndices;

    Vma m_vma;

    std::unique_ptr<SwapChain> m_swapchain;

    vk::UniqueRenderPass m_renderPass;
    vk::UniqueDescriptorSetLayout m_descriptorSetLayout;
    vk::UniquePipelineLayout m_pipelineLayout;
    vk::UniquePipeline m_graphicsPipeline;

    vk::UniqueCommandPool m_commandPool;
    std::unique_ptr<VmaBuffer> m_vertexBuffer;
    std::unique_ptr<VmaBuffer> m_indexBuffer;
    vk::UniqueDescriptorPool m_descriptorPool;
    std::vector<vk::UniqueDescriptorSet> m_descriptorSets;

    std::unique_ptr<VmaImage> m_textureImage;

    vk::Queue m_graphicsQueue;
    vk::Queue m_presentQueue;

    std::array<std::optional<FrameData<UniformBufferObject>>, MaxFramesInFlight> m_frameDatas;
    uint32_t m_currentFrame;

    bool m_framebufferResized;

    void InitWindow();

    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    bool IsDeviceSuitable(vk::PhysicalDevice device) const;
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateVma();
    void CreateSwapChain();
    void ReCreateSwapChain();
    void CreateRenderPass();
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    void CreateFrameBuffers();
    void CreateCommandPool();
    void CreateTextureImage(UploadQueue &uploadQueue);
    void CreateVertexBuffer(UploadQueue &uploadQueue);
    void CreateFrameData();
    void CreateDescriptorPool();
    void CreateDescriptorSets();
    void RecordCommandBuffer(FrameData<UniformBufferObject> &frameData, uint32_t imageIndex);

    void InitVulkan();
    void MainLoop();
    void UpdateUniformBuffer(FrameData<UniformBufferObject> &frameData);
    void DrawFrame();
    void Cleanup();
};

} // namespace zvk
