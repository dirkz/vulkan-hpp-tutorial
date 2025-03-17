#include "SwapChainSupportDetails.h"

namespace zvk
{

SwapChainSupportDetails::SwapChainSupportDetails(const vk::PhysicalDevice device,
                                                 const vk::SurfaceKHR surface)
{
    m_capabilities = device.getSurfaceCapabilitiesKHR(surface);
    m_formats = device.getSurfaceFormatsKHR(surface);
    m_presentModes = device.getSurfacePresentModesKHR(surface);
}

vk::SurfaceFormatKHR SwapChainSupportDetails::ChooseSurfaceFormat() const
{
    for (const vk::SurfaceFormatKHR &format : m_formats)
    {
        if (format.format == vk::Format::eB8G8R8A8Srgb &&
            format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return format;
        }
    }

    return m_formats[0];
}

vk::PresentModeKHR SwapChainSupportDetails::ChoosePresentMode() const
{
    for (const vk::PresentModeKHR &presentMode : m_presentModes)
    {
        if (presentMode == vk::PresentModeKHR::eMailbox)
        {
            return presentMode;
        }
    }
    return vk::PresentModeKHR::eFifo;
}

vk::SurfaceCapabilitiesKHR SwapChainSupportDetails::Capabilities() const
{
    return m_capabilities;
}

uint32_t SwapChainSupportDetails::ImageCount() const
{
    uint32_t imageCount = m_capabilities.minImageCount + 1;

    uint32_t maxImageCount = m_capabilities.maxImageCount;
    if (maxImageCount > 0 && imageCount > maxImageCount)
    {
        imageCount = maxImageCount;
    }

    return imageCount;
}

bool SwapChainSupportDetails::IsAdequate() const
{
    return !m_formats.empty() && !m_presentModes.empty();
}

} // namespace zvk