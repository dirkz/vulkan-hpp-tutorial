#pragma once

#include "stdafx.h"

#include "Vma.h"

namespace zvk
{

struct VmaImage
{
    VmaImage(const VmaAllocator allocator, vk::ImageCreateInfo createInfo);
    VmaImage(VmaImage &&other) noexcept;

    ~VmaImage();

  private:
    VmaAllocator m_allocator;
    VkImage m_image;
    VmaAllocation m_allocation;
    VmaAllocationInfo m_allocationInfo;
};

} // namespace zvk
