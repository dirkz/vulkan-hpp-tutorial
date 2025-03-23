#pragma once

#include "stdafx.h"

#include "VmaBuffer.h"

namespace zvk
{

template <class T> struct UniformBuffer
{
    UniformBuffer(const VmaAllocator allocator)
        : m_allocator{allocator}, m_stagingBuffer{VK_NULL_HANDLE},
          m_stagingAllocation{VK_NULL_HANDLE}, m_stagingAllocationInfo{}
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

        if (!m_isHostVisible)
        {
            // Allocation ended up in a non-mappable memory -
            // a transfer using a staging buffer is required.

            vk::BufferUsageFlags stagingBufferUsageFlags = vk::BufferUsageFlagBits::eTransferSrc;
            vk::BufferCreateInfo stagingBufferCreateInfo{{}, sizeof(T), stagingBufferUsageFlags};
            VkBufferCreateInfo stagingBufCreateInfo = stagingBufferCreateInfo;

            VmaAllocationCreateInfo stagingAllocCreateInfo = {};
            stagingAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
            stagingAllocCreateInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                                           VMA_ALLOCATION_CREATE_MAPPED_BIT;

            VkResult result =
                vmaCreateBuffer(m_allocator, &stagingBufCreateInfo, &stagingAllocCreateInfo,
                                &m_stagingBuffer, &m_stagingAllocation, &m_stagingAllocationInfo);

            vk::detail::resultCheck(vk::Result{result},
                                    "UniformBuffer(): vmaCreateBuffer failed (staging buffer)");
        }
    }

    ~UniformBuffer()
    {
        if (m_allocator != VK_NULL_HANDLE)
        {
            vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
        }
        if (m_stagingAllocation != VK_NULL_HANDLE)
        {
            vmaDestroyBuffer(m_allocator, m_stagingBuffer, m_stagingAllocation);
        }
    }

    static void MemoryBarrier(vk::CommandBuffer commandBuffer, vk::AccessFlags srcAccessMask,
                              vk::AccessFlags dstAccessMask, VkBuffer buffer,
                              vk::PipelineStageFlags srcStageMask,
                              vk::PipelineStageFlags dstStageMask)
    {
        vk::BufferMemoryBarrier bufferMemoryBarrier{srcAccessMask,
                                                    dstAccessMask,
                                                    VK_QUEUE_FAMILY_IGNORED,
                                                    VK_QUEUE_FAMILY_IGNORED,
                                                    buffer,
                                                    0,
                                                    VK_WHOLE_SIZE};

        vk::DependencyFlags dependencyFlags{};
        commandBuffer.pipelineBarrier(srcStageMask, dstStageMask, dependencyFlags, {},
                                      {bufferMemoryBarrier}, {});
    }

    inline void Update(vk::CommandBuffer commandBuffer, const T &uniform) const
    {
        if (m_isHostVisible)
        {
            // Allocation ended up in a mappable memory and is already mapped -
            // write to it directly.

            memcpy(m_allocInfo.pMappedData, &uniform, sizeof(T));
            VkResult result = vmaFlushAllocation(m_allocator, m_allocation, 0, VK_WHOLE_SIZE);

            vk::detail::resultCheck(vk::Result{result},
                                    "UniformBuffer::Update(): vmaFlushAllocation failed");

            vk::AccessFlags srcAccessMask = vk::AccessFlagBits::eHostWrite;
            vk::AccessFlags dstAccessMask = vk::AccessFlagBits::eUniformRead;
            vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlagBits::eHost;
            vk::PipelineStageFlags dstStageMask = vk::PipelineStageFlagBits::eVertexShader;
            MemoryBarrier(commandBuffer, srcAccessMask, dstAccessMask, m_buffer, srcStageMask,
                          dstStageMask);
        }
        else
        {
            // Allocation ended up in a non-mappable memory -
            // a transfer using a staging buffer is

            memcpy(m_stagingAllocationInfo.pMappedData, &uniform, sizeof(T));
            VkResult result =
                vmaFlushAllocation(m_allocator, m_stagingAllocation, 0, VK_WHOLE_SIZE);

            vk::detail::resultCheck(vk::Result{result},
                                    "UniformBuffer::Update(): vmaFlushAllocation failed");

            vk::AccessFlags srcAccessMask = vk::AccessFlagBits::eHostWrite;
            vk::AccessFlags dstAccessMask = vk::AccessFlagBits::eTransferRead;
            vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlagBits::eHost;
            vk::PipelineStageFlags dstStageMask = vk::PipelineStageFlagBits::eTransfer;
            MemoryBarrier(commandBuffer, srcAccessMask, dstAccessMask, m_stagingBuffer,
                          srcStageMask, dstStageMask);

            vk::BufferCopy bufCopy{
                0,         // srcOffset
                0,         // dstOffset,
                sizeof(T), // size
            };

            commandBuffer.copyBuffer(m_stagingBuffer, m_buffer, {bufCopy});

            vk::AccessFlags srcAccessMask2 = vk::AccessFlagBits::eTransferWrite;
            vk::AccessFlags dstAccessMask2 = vk::AccessFlagBits::eUniformRead;
            vk::PipelineStageFlags srcStageMask2 = vk::PipelineStageFlagBits::eTransfer;
            vk::PipelineStageFlags dstStageMask2 = vk::PipelineStageFlagBits::eVertexShader;
            MemoryBarrier(commandBuffer, srcAccessMask2, dstAccessMask2, m_buffer, srcStageMask2,
                          dstStageMask2);
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

    VkBuffer m_stagingBuffer;
    VmaAllocation m_stagingAllocation;
    VmaAllocationInfo m_stagingAllocationInfo;
};

} // namespace zvk
