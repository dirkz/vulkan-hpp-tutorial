#pragma once

#include "stdafx.h"

#include "VmaBuffer.h"
#include "VmaIncludes.h"

namespace zvk
{

struct MappedBuffer : public VmaBuffer
{
    MappedBuffer(VmaAllocator allocator, vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                 vk::SharingMode sharingMode);
};

} // namespace zvk
