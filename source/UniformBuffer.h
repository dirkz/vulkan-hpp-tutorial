#pragma once

#include "stdafx.h"

#include "VmaBuffer.h"

namespace zvk
{

template <class T> struct UniformBuffer
{
    UniformBuffer(const VmaAllocator allocator) : m_allocator{allocator}
    {
        vk::BufferUsageFlags bufferUsageFlags =
            vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eTransferDst;
        vk::BufferCreateInfo bufferCreateInfo{{}, sizeof(T), bufferUsageFlags};

        VkBufferCreateInfo vkBufferCreateInfo = bufferCreateInfo;

        VmaAllocationCreateInfo allocCreateInfo = {};
        allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
        allocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                                VMA_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT |
                                VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VkResult result = vmaCreateBuffer(allocator, &vkBufferCreateInfo, &allocCreateInfo,
                                          &m_buffer, &m_allocation, &m_allocInfo);

        vk::detail::resultCheck(vk::Result{result}, "UniformBuffer(): vmaCreateBuffer failed");

        VkMemoryPropertyFlags memPropFlags;
        vmaGetAllocationMemoryProperties(allocator, m_allocation, &memPropFlags);

        m_isHostVisible = memPropFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    }

    ~UniformBuffer()
    {
        if (m_allocator != VK_NULL_HANDLE)
        {
            vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
        }
    }

    inline void Update(vk::CommandBuffer commandBuffer, const T &uniform) const
    {
        if (m_isHostVisible)
        {
            // Allocation ended up in a mappable memory and is already mapped - write to it
            // directly.

            memcpy(m_allocInfo.pMappedData, &uniform, sizeof(T));
            VkResult result = vmaFlushAllocation(m_allocator, m_allocation, 0, VK_WHOLE_SIZE);

            vk::detail::resultCheck(vk::Result{result},
                                    "UniformBuffer::Update(): vmaFlushAllocation failed");

            VkBufferMemoryBarrier bufMemBarrier = {VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER};
            bufMemBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            bufMemBarrier.dstAccessMask = VK_ACCESS_UNIFORM_READ_BIT;
            bufMemBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            bufMemBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            bufMemBarrier.buffer = m_buffer;
            bufMemBarrier.offset = 0;
            bufMemBarrier.size = VK_WHOLE_SIZE;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_HOST_BIT,
                                 VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, 0, 0, nullptr, 1,
                                 &bufMemBarrier, 0, nullptr);
        }
    }

    inline VkBuffer Buffer() const
    {
        return m_buffer;
    }

  private:
    VmaAllocator m_allocator;
    VkBuffer m_buffer;
    VmaAllocation m_allocation;
    VmaAllocationInfo m_allocInfo;
    bool m_isHostVisible;
};

} // namespace zvk
