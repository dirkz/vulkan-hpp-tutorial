#pragma once

#include "stdafx.h"

#include "VmaIncludes.h"

namespace zvk
{

struct VmaBuffer
{
    VmaBuffer(const VmaAllocator allocator, vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
              VmaAllocationCreateFlags allocationCreateFlags, vk::SharingMode sharingMode);
    ~VmaBuffer();

    inline VkBuffer Buffer() const
    {
        return m_buffer;
    }

  protected:
    VmaAllocator m_allocator;
    VkBuffer m_buffer;
    VmaAllocation m_allocation;
    VmaAllocationInfo m_allocationInfo;
};

} // namespace zvk
