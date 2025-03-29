#include "UploadQueue.h"

namespace zvk
{

UploadQueue::UploadQueue(const vk::Device device, uint32_t uploadQueue) : m_device{device}
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

void UploadQueue::FinishAndWait()
{
    m_commandBuffer->end();
    vk::SubmitInfo submitInfo{{}, {}, {*m_commandBuffer}, {}};
    m_queue.submit({submitInfo}, *m_fence);
    vk::Result result = m_device.waitForFences({*m_fence}, VK_TRUE, UINT64_MAX);
    assert(result == vk::Result::eSuccess);
}

} // namespace zvk