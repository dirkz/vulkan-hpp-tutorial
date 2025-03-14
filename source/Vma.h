#pragma once

#include "stdafx.h"

#define VMA_VULKAN_VERSION 1002000 // Vulkan 1.2
#include <vk_mem_alloc.h>

namespace zvk
{

struct Vma
{
    Vma();
    Vma(vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device);
    Vma(const Vma &vma) = delete;
    Vma(Vma &&vma) = delete;

    ~Vma();

    void Reset(vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device);

    inline VmaAllocator Allocator() const
    {
        assert(m_allocator != VK_NULL_HANDLE);
        return m_allocator;
    }

  private:
    VmaAllocator m_allocator;

    void Delete();
};

} // namespace zvk
