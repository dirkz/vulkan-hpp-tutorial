#include "UploadQueue.h"

#include "VmaImage.h"

namespace zvk
{

UploadQueue::UploadQueue(const Vma &vma, const vk::Device device, uint32_t uploadQueue)
    : m_vma{vma}, m_device{device}
{
    m_queue = device.getQueue(uploadQueue, 0);

    vk::FenceCreateInfo fenceCreateInfo{};
    m_fence = device.createFenceUnique(fenceCreateInfo);

    vk::CommandPoolCreateFlags commandPoolCreateFlags =
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer |
        vk::CommandPoolCreateFlagBits::eTransient;
    vk::CommandPoolCreateInfo createInfo{commandPoolCreateFlags, uploadQueue};
    m_commandPool = device.createCommandPoolUnique(createInfo);

    vk::CommandBufferAllocateInfo allocInfo{m_commandPool.get(), vk::CommandBufferLevel::ePrimary,
                                            1};
    auto commandBuffers = device.allocateCommandBuffersUnique(allocInfo);
    m_commandBuffer = std::move(commandBuffers[0]);

    vk::CommandBufferBeginInfo beginInfo{};
    m_commandBuffer->begin(beginInfo);
}

VmaImage UploadQueue::UploadImage(int width, int height, int size, void *pImageData)
{
    MappedBuffer stagingBuffer = m_vma.CreateMappedBuffer(
        size, vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive);
    memcpy(stagingBuffer.Mapped(), pImageData, size);

    vk::Extent3D imageExtent{static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};
    constexpr uint32_t mipLayers = 1;
    constexpr uint32_t arrayLayers = 1;
    vk::ImageCreateInfo imageCreateInfo{{},
                                        vk::ImageType::e2D,
                                        vk::Format::eR8G8B8A8Srgb,
                                        imageExtent,
                                        mipLayers,
                                        arrayLayers,
                                        vk::SampleCountFlagBits::e1,
                                        vk::ImageTiling::eOptimal,
                                        vk::ImageUsageFlagBits::eTransferDst |
                                            vk::ImageUsageFlagBits::eSampled,
                                        vk::SharingMode::eExclusive,
                                        {},
                                        vk::ImageLayout::eUndefined};

    VmaImage image{m_vma.Allocator(), imageCreateInfo};

    return image;
}

void UploadQueue::FinishAndWait()
{
    m_commandBuffer->end();
    vk::SubmitInfo submitInfo{{}, {}, {*m_commandBuffer}, {}};
    m_queue.submit({submitInfo}, *m_fence);
    vk::Result result = m_device.waitForFences({*m_fence}, VK_TRUE, UINT64_MAX);
    assert(result == vk::Result::eSuccess);
}

} // namespace zvk