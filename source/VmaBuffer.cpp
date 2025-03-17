#include "VmaBuffer.h"

namespace zvk
{

VmaBuffer::VmaBuffer(VmaAllocator allocator, vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                     VmaAllocationCreateFlags allocationCreateFlags, vk::SharingMode sharingMode)
    : m_allocator{allocator}
{
    vk::BufferCreateInfo bufferCreateInfo{{}, size, usageFlags, sharingMode};

    VmaAllocationCreateInfo allocationCreateInfo{allocationCreateFlags, VMA_MEMORY_USAGE_AUTO};

    VkBufferCreateInfo vkBufferCreateInfo = bufferCreateInfo;
    VkResult result = vmaCreateBuffer(allocator, &vkBufferCreateInfo, &allocationCreateInfo,
                                      &m_buffer, &m_allocation, &m_allocationInfo);

    vk::detail::resultCheck(vk::Result{result}, "vmaCreateBuffer");
}

VmaBuffer::~VmaBuffer()
{
    vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
}

} // namespace zvk