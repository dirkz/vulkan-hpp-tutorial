#include "BufferTransfer.h"

namespace zvk
{

BufferTransfer::BufferTransfer(const vk::Device device, const QueueFamilyIndices *familyIndices)
{
    vk::CommandPoolCreateInfo createInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                                         familyIndices->GraphicsFamily().value()};
    m_commandPool = device.createCommandPoolUnique(createInfo);
}

} // namespace zvk