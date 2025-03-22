#pragma once

#include "stdafx.h"

#include "VmaBuffer.h"

namespace zvk
{

template <class T> struct UniformBuffer : VmaBuffer
{
    UniformBuffer(const VmaAllocator allocator, vk::BufferUsageFlags usageFlags,
                  vk::SharingMode sharingMode)
        : VmaBuffer{allocator, sizeof(T), usageFlags,
                    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                        VMA_ALLOCATION_CREATE_MAPPED_BIT,
                    sharingMode} {};

    inline T *Mapped() const
    {
        return static_cast<T *>(m_allocationInfo.pMappedData);
    }

    inline void Update(const T &uniform) const
    {
        memcpy(Mapped(), &uniform, sizeof(T));
    }
};

} // namespace zvk
