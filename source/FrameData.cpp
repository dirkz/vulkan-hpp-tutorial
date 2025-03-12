#include "FrameData.h"

namespace zvk
{

FrameData::FrameData(const vk::Device &device, const vk::CommandPool &pool)
{
    vk::CommandBufferAllocateInfo commandBufferAllocateInfo{pool, vk::CommandBufferLevel::ePrimary,
                                                            1};
    auto commandBuffers = device.allocateCommandBuffersUnique(commandBufferAllocateInfo);
    m_commandBuffer = std::move(commandBuffers[0]);

    vk::SemaphoreCreateInfo semaphoreCreateInfo{};
    m_imageAvailableSemaphore = device.createSemaphoreUnique(semaphoreCreateInfo);
    m_renderFinishedSemaphore = device.createSemaphoreUnique(semaphoreCreateInfo);

    vk::FenceCreateInfo fenceCreateInfo{vk::FenceCreateFlagBits::eSignaled};
    m_inFlightFence = device.createFenceUnique(fenceCreateInfo);
}

} // namespace zvk