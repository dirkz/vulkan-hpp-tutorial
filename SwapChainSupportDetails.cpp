#include "SwapChainSupportDetails.h"

namespace zvk
{

SwapChainSupportDetails::SwapChainSupportDetails(vk::PhysicalDevice device, vk::SurfaceKHR surface)
{
    m_capabilities = device.getSurfaceCapabilitiesKHR(surface);
    m_formats = device.getSurfaceFormatsKHR(surface);
    m_presentModes = device.getSurfacePresentModesKHR(surface);
}

vk::SurfaceFormatKHR SwapChainSupportDetails::ChooseSurfaceFormat()
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

vk::PresentModeKHR SwapChainSupportDetails::ChoosePresentMode()
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

vk::Extent2D SwapChainSupportDetails::ChooseSwapExtent()
{
    return vk::Extent2D();
}

bool SwapChainSupportDetails::IsAdequate()
{
    return !m_formats.empty() && !m_presentModes.empty();
}

} // namespace zvk