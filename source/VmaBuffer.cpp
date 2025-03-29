#include "VmaBuffer.h"

namespace zvk
{

VmaBuffer::VmaBuffer(const VmaAllocator allocator, vk::DeviceSize size,
                     vk::BufferUsageFlags usageFlags,
                     VmaAllocationCreateFlags allocationCreateFlags, vk::SharingMode sharingMode,
                     const std::span<uint32_t> &queues)
    : m_size{size}, m_allocator{allocator}
{
    vk::BufferCreateInfo bufferCreateInfo{{}, size, usageFlags, sharingMode, queues};

    VmaAllocationCreateInfo allocationCreateInfo{allocationCreateFlags, VMA_MEMORY_USAGE_AUTO};

    VkBufferCreateInfo vkBufferCreateInfo = bufferCreateInfo;
    VkResult result = vmaCreateBuffer(allocator, &vkBufferCreateInfo, &allocationCreateInfo,
                                      &m_buffer, &m_allocation, &m_allocationInfo);

    vk::detail::resultCheck(vk::Result{result}, "VmaBuffer(): vmaCreateBuffer failed");
}

VmaBuffer::VmaBuffer(VmaBuffer &&other) noexcept
    : m_allocator{other.m_allocator}, m_buffer{other.m_buffer}, m_allocation{other.m_allocation},
      m_allocationInfo{other.m_allocationInfo}, m_size{other.m_size}
{
    other.m_allocator = VK_NULL_HANDLE;
}

VmaBuffer::~VmaBuffer()
{
    if (m_allocator != VK_NULL_HANDLE)
    {
        vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
    }
}

} // namespace zvk