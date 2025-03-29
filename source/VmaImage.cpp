#include "VmaImage.h"

namespace zvk
{

VmaImage::VmaImage(const VmaAllocator allocator, vk::ImageCreateInfo createInfo)
    : m_allocator{allocator}
{
    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
    allocCreateInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
    allocCreateInfo.priority = 1.0f;

    VkImageCreateInfo imageCreateInfo = createInfo;
    vmaCreateImage(allocator, &imageCreateInfo, &allocCreateInfo, &m_image, &m_allocation,
                   &m_allocationInfo);
}

VmaImage::VmaImage(VmaImage &&other) noexcept
    : m_allocator{other.m_allocator}, m_image{other.m_image}, m_allocation{other.m_allocation},
      m_allocationInfo{other.m_allocationInfo}
{
    other.m_allocator = VK_NULL_HANDLE;
}

VmaImage::~VmaImage()
{
    if (m_allocator != VK_NULL_HANDLE)
    {
        vmaDestroyImage(m_allocator, m_image, m_allocation);
    }
}

} // namespace zvk