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

MappedBuffer Vma::CreateMappedBuffer(const vk::DeviceSize size,
                                     const vk::BufferUsageFlags usageFlags,
                                     const vk::SharingMode sharingMode)
{
    return MappedBuffer{m_allocator, size, usageFlags, sharingMode};
}

MappedBuffer *Vma::NewMappedBuffer(vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                                   vk::SharingMode sharingMode)
{
    // Re-use, even though it means we have to move, which is cheap.
    MappedBuffer buffer = CreateMappedBuffer(size, usageFlags, sharingMode);
    return new MappedBuffer{std::move(buffer)};
}

VmaBuffer Vma::CreateDeviceLocalBuffer(const vk::DeviceSize size,
                                       const vk::BufferUsageFlags usageFlags,
                                       const vk::SharingMode sharingMode,
                                       const std::vector<uint32_t> &queues)
{
    VmaBuffer buffer = VmaBuffer{m_allocator, size, usageFlags, 0, sharingMode, queues};
    return buffer;
}

VmaBuffer *Vma::NewDeviceLocalBuffer(vk::DeviceSize size, vk::BufferUsageFlags usageFlags,
                                     vk::SharingMode sharingMode,
                                     const std::vector<uint32_t> &queues)
{
    // Re-use, even though it means we have to move, which is cheap.
    VmaBuffer buffer = CreateDeviceLocalBuffer(size, usageFlags, sharingMode, queues);
    return new VmaBuffer{std::move(buffer)};
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
