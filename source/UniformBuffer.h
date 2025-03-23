#pragma once

#include "stdafx.h"

#include "VmaBuffer.h"

namespace zvk
{

template <class T> struct UniformBuffer
{
    UniformBuffer(const VmaAllocator allocator)
    {
    }

    inline T *Mapped() const
    {
        return nullptr;
    }

    inline void Update(const T &uniform) const
    {
    }

    inline VkBuffer Buffer() const
    {
        return VK_NULL_HANDLE;
    }
};

} // namespace zvk
