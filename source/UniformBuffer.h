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
        m_isHostVisible = false;

        if (!m_isHostVisible)
        {
            // Allocation ended up in a non-mappable memory - a transfer using a staging buffer is
            // required.
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
        else
        {
            memcpy(m_stagingAllocationInfo.pMappedData, &uniform, sizeof(T));
            VkResult result =
                vmaFlushAllocation(m_allocator, m_stagingAllocation, 0, VK_WHOLE_SIZE);

            vk::detail::resultCheck(vk::Result{result},
                                    "UniformBuffer::Update(): vmaFlushAllocation failed");

            vk::AccessFlags srcAccessMask = vk::AccessFlagBits::eHostWrite;
            vk::AccessFlags dstAccessMask = vk::AccessFlagBits::eTransferRead;
            vk::BufferMemoryBarrier bufferMemoryBarrier{srcAccessMask,
                                                        dstAccessMask,
                                                        VK_QUEUE_FAMILY_IGNORED,
                                                        VK_QUEUE_FAMILY_IGNORED,
                                                        m_stagingBuffer,
                                                        0,
                                                        VK_WHOLE_SIZE};

            vk::PipelineStageFlags srcStageMask = vk::PipelineStageFlagBits::eHost;
            vk::PipelineStageFlags dstStageMask = vk::PipelineStageFlagBits::eTransfer;
            vk::DependencyFlags dependencyFlags{};
            commandBuffer.pipelineBarrier(srcStageMask, dstStageMask, dependencyFlags, {},
                                          {bufferMemoryBarrier}, {});

            vk::BufferCopy bufCopy{
                0,         // srcOffset
                0,         // dstOffset,
                sizeof(T), // size
            };

            commandBuffer.copyBuffer(m_stagingBuffer, m_buffer, {bufCopy});

            vk::AccessFlags srcAccessMask2 = vk::AccessFlagBits::eTransferWrite;
            vk::AccessFlags dstAccessMask2 = vk::AccessFlagBits::eUniformRead;
            vk::BufferMemoryBarrier bufferMemoryBarrier2{srcAccessMask2,
                                                         dstAccessMask2,
                                                         VK_QUEUE_FAMILY_IGNORED,
                                                         VK_QUEUE_FAMILY_IGNORED,
                                                         m_buffer,
                                                         0,
                                                         VK_WHOLE_SIZE};

            vk::PipelineStageFlags srcStageMask2 = vk::PipelineStageFlagBits::eTransfer;
            vk::PipelineStageFlags dstStageMask2 = vk::PipelineStageFlagBits::eVertexShader;
            vk::DependencyFlags dependencyFlags2{};
            commandBuffer.pipelineBarrier(srcStageMask2, dstStageMask2, dependencyFlags2, {},
                                          {bufferMemoryBarrier2}, {});
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
