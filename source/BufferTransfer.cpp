#include "BufferTransfer.h"

namespace zvk
{

BufferTransfer::BufferTransfer(const vk::Device device, const QueueFamilyIndices *familyIndices)
    : m_device{device}
{
    m_queue = device.getQueue(familyIndices->TransferFamily().value(), 0);

    vk::FenceCreateInfo fenceCreateInfo{};
    m_fence = device.createFenceUnique(fenceCreateInfo);

    vk::CommandPoolCreateFlags commandPoolCreateFlags =
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer |
        vk::CommandPoolCreateFlagBits::eTransient;
    vk::CommandPoolCreateInfo createInfo{commandPoolCreateFlags,
                                         familyIndices->TransferFamily().value()};
    m_commandPool = device.createCommandPoolUnique(createInfo);

    vk::CommandBufferAllocateInfo allocInfo{m_commandPool.get(), vk::CommandBufferLevel::ePrimary,
                                            1};
    auto commandBuffers = device.allocateCommandBuffersUnique(allocInfo);
    m_commandBuffer = std::move(commandBuffers[0]);

    vk::CommandBufferBeginInfo beginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit};
    m_commandBuffer->begin(beginInfo);
}

void BufferTransfer::Copy(const VmaBuffer &srcBuffer, const VmaBuffer &dstBuffer)
{
    vk::BufferCopy copyRegion{0, 0, srcBuffer.Size()};
    m_commandBuffer->copyBuffer(srcBuffer.Buffer(), dstBuffer.Buffer(), {copyRegion});
}

void BufferTransfer::FinishAndWait()
{
    m_commandBuffer->end();
    vk::SubmitInfo submitInfo{{}, {}, {*m_commandBuffer}, {}};
    m_queue.submit({submitInfo}, *m_fence);
    vk::Result result = m_device.waitForFences({*m_fence}, VK_TRUE, UINT64_MAX);
    assert(result == vk::Result::eSuccess);
}

} // namespace zvk