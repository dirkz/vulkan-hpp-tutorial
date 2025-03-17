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
                                    vk::SharingMode sharingMode);

    VmaBuffer CreateDeviceLocalBuffer(vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                                      vk::SharingMode sharingMode,
                                      const std::vector<uint32_t> &queues = {});

  private:
    VmaAllocator m_allocator;

    void Delete();
};

} // namespace zvk
