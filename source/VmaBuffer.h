#pragma once

#include "stdafx.h"

#include "VmaIncludes.h"

namespace zvk
{

struct VmaBuffer
{
    VmaBuffer(VmaAllocator allocator, vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
              VmaAllocationCreateFlags allocationCreateFlags, vk::SharingMode sharingMode);
    VmaBuffer(VmaBuffer &&buffer) noexcept;
    ~VmaBuffer();

    inline void *Mapped() const
    {
        return m_allocationInfo.pMappedData;
    }

    inline VkBuffer Buffer() const
    {
        return m_buffer;
    }

  private:
    VmaAllocator m_allocator;
    VkBuffer m_buffer;
    VmaAllocation m_allocation;
    VmaAllocationInfo m_allocationInfo;
};

} // namespace zvk
