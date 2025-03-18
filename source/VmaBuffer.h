#pragma once

#include "stdafx.h"

#include "VmaIncludes.h"

namespace zvk
{

struct VmaBuffer
{
    VmaBuffer(const VmaAllocator allocator, vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
              VmaAllocationCreateFlags allocationCreateFlags, vk::SharingMode sharingMode,
              const std::span<uint32_t> &queues = {});
    VmaBuffer(VmaBuffer &&buffer) noexcept;

    ~VmaBuffer();

    inline VkBuffer Buffer() const
    {
        return m_buffer;
    }

    inline vk::DeviceSize Size() const
    {
        return m_size;
    }

  protected:
    vk::DeviceSize m_size;
    VmaAllocator m_allocator;
    VkBuffer m_buffer;
    VmaAllocation m_allocation;
    VmaAllocationInfo m_allocationInfo;
};

} // namespace zvk
