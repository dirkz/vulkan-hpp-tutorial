#include "SwapChainSupportDetails.h"

namespace zvk
{

SwapChainSupportDetails::SwapChainSupportDetails(vk::PhysicalDevice device, vk::SurfaceKHR surface)
{
    m_capabilities = device.getSurfaceCapabilitiesKHR(surface);
    m_formats = device.getSurfaceFormatsKHR(surface);
    m_presentModes = device.getSurfacePresentModesKHR(surface);
}

bool SwapChainSupportDetails::IsAdequate()
{
    return !m_formats.empty() && !m_presentModes.empty();
}

} // namespace zvk