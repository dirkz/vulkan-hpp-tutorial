#include "BufferTransfer.h"

namespace zvk
{

BufferTransfer::BufferTransfer(const vk::Device device, const QueueFamilyIndices *familyIndices)
{
    vk::CommandPoolCreateFlags commandPoolCreateFlags =
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer |
        vk::CommandPoolCreateFlagBits::eTransient;
    vk::CommandPoolCreateInfo createInfo{commandPoolCreateFlags,
                                         familyIndices->GraphicsFamily().value()};
    m_commandPool = device.createCommandPoolUnique(createInfo);

    vk::CommandBufferAllocateInfo allocInfo{m_commandPool.get(), vk::CommandBufferLevel::ePrimary,
                                            1};
    auto commandBuffers = device.allocateCommandBuffersUnique(allocInfo);
    m_commandBuffer = std::move(commandBuffers[0]);
}

} // namespace zvk