#pragma once

#include "stdafx.h"

#include "MappedBuffer.h"
#include "VmaIncludes.h"

namespace zvk
{

struct Vma
{
    Vma();
    Vma(const vk::Instance instance, const vk::PhysicalDevice physicalDevice,
        const vk::Device device);
    Vma(const Vma &vma) = delete;
    Vma(Vma &&vma) = delete;

    ~Vma();

    void Reset(const vk::Instance instance, const vk::PhysicalDevice physicalDevice,
               const vk::Device device);

    inline VmaAllocator Allocator() const
    {
        assert(m_allocator != VK_NULL_HANDLE);
        return m_allocator;
    }

    MappedBuffer CreateMappedBuffer(vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                                    vk::SharingMode sharingMode) const;

    MappedBuffer *NewMappedBuffer(vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                                  vk::SharingMode sharingMode) const;

    VmaBuffer CreateDeviceLocalBuffer(vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                                      vk::SharingMode sharingMode,
                                      const std::span<uint32_t> &queues = {}) const;

    VmaBuffer *NewDeviceLocalBuffer(vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                                    vk::SharingMode sharingMode,
                                    const std::span<uint32_t> &queues = {}) const;

  private:
    VmaAllocator m_allocator;

    void Delete();
};

} // namespace zvk
