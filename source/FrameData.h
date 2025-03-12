#pragma once

#include "stdafx.h"

namespace zvk
{

struct FrameData
{
    FrameData(const vk::Device &device, const vk::CommandPool &pool);

    inline const vk::CommandBuffer &CommandBuffer() const
    {
        return m_commandBuffer.get();
    }

    inline const vk::Semaphore &ImageAvailableSemaphore() const
    {
        return m_imageAvailableSemaphore.get();
    }

    inline const vk::Semaphore &RenderFinishedSemaphore() const
    {
        return m_renderFinishedSemaphore.get();
    }

    inline const vk::Fence &InFlightFence() const
    {
        return m_inFlightFence.get();
    }

  private:
    vk::UniqueCommandBuffer m_commandBuffer;
    vk::UniqueSemaphore m_imageAvailableSemaphore;
    vk::UniqueSemaphore m_renderFinishedSemaphore;
    vk::UniqueFence m_inFlightFence;
};

} // namespace zvk
