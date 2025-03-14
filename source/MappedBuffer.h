#pragma once

#include "stdafx.h"

#include "Vma.h"

namespace zvk
{

struct MappedBuffer
{
    MappedBuffer(VmaAllocator allocator, vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                 vk::SharingMode sharingMode);
    ~MappedBuffer();

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
