#pragma once

#include "stdafx.h"

namespace zvk
{

struct SwapChainSupportDetails
{
    SwapChainSupportDetails(const vk::PhysicalDevice device, const vk::SurfaceKHR surface);

    vk::SurfaceFormatKHR ChooseSurfaceFormat() const;
    vk::PresentModeKHR ChoosePresentMode() const;
    vk::SurfaceCapabilitiesKHR Capabilities() const;

    uint32_t ImageCount() const;

    bool IsAdequate() const;

  private:
    vk::SurfaceCapabilitiesKHR m_capabilities;
    std::vector<vk::SurfaceFormatKHR> m_formats;
    std::vector<vk::PresentModeKHR> m_presentModes;
};

} // namespace zvk
