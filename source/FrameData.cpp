#include "FrameData.h"

namespace zvk
{

FrameData::FrameData(const vk::Device &device, const vk::CommandPool &pool)
{
    vk::CommandBufferAllocateInfo info{pool, vk::CommandBufferLevel::ePrimary, 1};
    auto commandBuffers = device.allocateCommandBuffersUnique(info);
    m_commandBuffer = std::move(commandBuffers[0]);
}

} // namespace zvk