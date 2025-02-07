#include "SwapChain.h"

#include "QueueFamilyIndices.h"
#include "SwapChainSupportDetails.h"

namespace zvk
{

SwapChain::SwapChain(const Window *pWindow, const vk::PhysicalDevice &physicalDevice,
                     const vk::Device &device, const vk::SurfaceKHR &surface)
{
    SwapChainSupportDetails support{physicalDevice, surface};

    vk::SurfaceFormatKHR format = support.ChooseSurfaceFormat();
    vk::PresentModeKHR presentMode = support.ChoosePresentMode();
    vk::Extent2D extent = pWindow->ChooseSwapExtent(support.Capabilities());

    uint32_t imageCount = support.ImageCount();

    QueueFamilyIndices indices{physicalDevice, surface};
    std::array<uint32_t, 2> queueFamilyIndices = {indices.GraphicsFamily().value(),
                                                  indices.PresentFamily().value()};
    std::vector<uint32_t> combinedIndices = indices.UniqueGraphicsAndPresent();

    // "Normal" case: both graphics and present use the same queue:
    vk::SharingMode imageSharingMode = vk::SharingMode::eExclusive;
    uint32_t queueFamilyIndexCount = 0;
    const uint32_t *pQueueFamilyIndices = nullptr;

    // Unless they are not using the same queue:
    if (combinedIndices.size() > 1)
    {
        imageSharingMode = vk::SharingMode::eConcurrent;
        queueFamilyIndexCount = queueFamilyIndices.size();
        pQueueFamilyIndices = queueFamilyIndices.data();
    }

    vk::SwapchainCreateInfoKHR createInfo{{},
                                          surface,
                                          imageCount,
                                          format.format,
                                          format.colorSpace,
                                          extent,
                                          1 /* imageArrayLayers */,
                                          vk::ImageUsageFlagBits::eColorAttachment,
                                          imageSharingMode,
                                          queueFamilyIndexCount,
                                          pQueueFamilyIndices,
                                          support.Capabilities().currentTransform,
                                          vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                          presentMode,
                                          VK_TRUE /* clipped */,
                                          VK_NULL_HANDLE /* oldSwapchain */};

    m_swapchain = device.createSwapchainKHRUnique(createInfo);
    m_images = device.getSwapchainImagesKHR(m_swapchain.get());
    m_format = format.format;
    m_extent = extent;

    CreateImageViews(device);
}

void SwapChain::CreateImageViews(const vk::Device &device)
{
    m_imageViews.resize(m_images.size());

    constexpr uint32_t baseMipLevel = 0;
    constexpr uint32_t levelCount = 1;
    constexpr uint32_t baseArrayLayer = 0;
    constexpr uint32_t layerCount = 1;
    constexpr vk::ImageSubresourceRange subresourceRange{
        vk::ImageAspectFlagBits::eColor, baseMipLevel, levelCount, baseArrayLayer, layerCount};

    constexpr vk::ComponentMapping componentMapping{}; // all vk::ComponentSwizzle::eIdentity

    for (int i = 0; i < m_images.size(); ++i)
    {
        vk::ImageViewCreateInfo createInfo{{},       m_images[i],      vk::ImageViewType::e2D,
                                           m_format, componentMapping, subresourceRange};
        m_imageViews[i] = device.createImageViewUnique(createInfo);
    }
}

void SwapChain::CreateFrameBuffers(const vk::Device &device, const vk::RenderPass &renderPass)
{
    m_frameBuffers.resize(m_imageViews.size());

    for (int i = 0; i < m_imageViews.size(); ++i)
    {
        vk::FramebufferCreateInfo createInfo{
            {}, renderPass, {m_imageViews[i].get()}, m_extent.width, m_extent.height, 1};
        m_frameBuffers[i] = device.createFramebufferUnique(createInfo);
    }
}

vk::Format SwapChain::Format() const
{
    return m_format;
}

vk::Extent2D SwapChain::Extent() const
{
    return m_extent;
}

} // namespace zvk