#pragma once

#include "stdafx.h"

#include "Window.h"

namespace zvk
{

struct SwapChain
{
    SwapChain(const Window *pWindow, const vk::PhysicalDevice &physicalDevice,
              const vk::Device &device, const vk::SurfaceKHR &surface);

    void CreateImageViews(const vk::Device &device);
    void CreateFrameBuffers(const vk::Device &device, const vk::RenderPass &renderPass);

    vk::Format Format() const;
    vk::Extent2D Extent() const;
    vk::Framebuffer FrameBuffer(const uint32_t index);

    inline vk::SwapchainKHR operator*()
    {
        return m_swapchain.get();
    }

  private:
    vk::UniqueSwapchainKHR m_swapchain;
    std::vector<vk::Image> m_images;
    vk::Format m_format;
    vk::Extent2D m_extent;
    std::vector<vk::UniqueImageView> m_imageViews;
    std::vector<vk::UniqueFramebuffer> m_frameBuffers;
};

} // namespace zvk
