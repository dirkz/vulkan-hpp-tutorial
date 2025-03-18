#pragma once

#include "stdafx.h"

#include "VmaBuffer.h"
#include "VmaIncludes.h"

namespace zvk
{

struct MappedBuffer : public VmaBuffer
{
    MappedBuffer(const VmaAllocator allocator, vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                 vk::SharingMode sharingMode);
    MappedBuffer(MappedBuffer &&) = default;

    ~MappedBuffer() = default;

    inline void *Mapped() const
    {
        return m_allocationInfo.pMappedData;
    }
};

} // namespace zvk
