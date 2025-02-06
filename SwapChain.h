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

  private:
    vk::UniqueSwapchainKHR m_swapchain;
    std::vector<vk::Image> m_images;
    vk::Format m_format;
    vk::Extent2D m_extent;
    std::vector<vk::UniqueImageView> m_imageViews;
};

} // namespace zvk
