#include "MappedBuffer.h"

namespace zvk
{

MappedBuffer::MappedBuffer(VmaAllocator allocator, vk::DeviceSize size,
                           vk::BufferUsageFlags usageFlags, vk::SharingMode sharingMode)
    : VmaBuffer{allocator, size, usageFlags,
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
                    VMA_ALLOCATION_CREATE_MAPPED_BIT,
                sharingMode}
{
}

} // namespace zvk