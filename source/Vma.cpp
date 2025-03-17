#define VMA_IMPLEMENTATION
#include "Vma.h"

namespace zvk
{

Vma::Vma() : m_allocator{VK_NULL_HANDLE}
{
}

Vma::Vma(vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device)
    : m_allocator{VK_NULL_HANDLE}
{
    Reset(instance, physicalDevice, device);
}

Vma::~Vma()
{
    Delete();
}

void Vma::Reset(vk::Instance instance, vk::PhysicalDevice physicalDevice, vk::Device device)
{
    Delete();

    auto getBufferMemoryRequirements2KHRFn =
        device.getProcAddr("vkGetBufferMemoryRequirements2KHR");

    VmaVulkanFunctions vulkanFunctions = {};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;
    vulkanFunctions.vkGetBufferMemoryRequirements2KHR =
        reinterpret_cast<PFN_vkGetBufferMemoryRequirements2KHR>(getBufferMemoryRequirements2KHRFn);

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_2;
    allocatorCreateInfo.physicalDevice = physicalDevice;
    allocatorCreateInfo.device = device;
    allocatorCreateInfo.instance = instance;
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    vmaCreateAllocator(&allocatorCreateInfo, &m_allocator);
}

MappedBuffer *Vma::CreateMappedBuffer(vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                                      vk::SharingMode sharingMode)
{
    return new MappedBuffer{m_allocator, size, usageFlags, sharingMode};
}

VmaBuffer *Vma::CreateDeviceLocalBuffer(vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                                        vk::SharingMode sharingMode)
{
    VmaBuffer *buffer = new VmaBuffer{m_allocator, size, usageFlags, 0, sharingMode};

    return buffer;
}

void Vma::Delete()
{
    if (m_allocator != VK_NULL_HANDLE)
    {
        vmaDestroyAllocator(m_allocator);
    }
    m_allocator = VK_NULL_HANDLE;
}

} // namespace zvk
