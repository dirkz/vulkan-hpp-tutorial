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
}

} // namespace zvk