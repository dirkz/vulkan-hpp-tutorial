#include "MappedBuffer.h"

namespace zvk
{

MappedBuffer::MappedBuffer(VmaAllocator allocator, vk::DeviceSize size,
                           vk::BufferUsageFlags usageFlags, vk::SharingMode sharingMode)
    : m_allocator{allocator}
{
    vk::BufferCreateInfo bufferCreateInfo{{}, size, usageFlags, sharingMode};

    VmaAllocationCreateFlags allocationCreateFlags =
        VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
    VmaAllocationCreateInfo allocationCreateInfo{allocationCreateFlags, VMA_MEMORY_USAGE_AUTO};

    VkBufferCreateInfo vkBufferCreateInfo = bufferCreateInfo;
    VkResult result = vmaCreateBuffer(allocator, &vkBufferCreateInfo, &allocationCreateInfo,
                                      &m_buffer, &m_allocation, &m_allocationInfo);

    vk::detail::resultCheck(vk::Result{result}, "vmaCreateBuffer");
}

MappedBuffer::~MappedBuffer()
{
    vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
}

} // namespace zvk