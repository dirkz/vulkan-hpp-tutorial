#pragma once

#include "stdafx.h"

namespace zvk
{

struct FrameData
{
    FrameData(const vk::Device &device, const vk::CommandPool &pool);

    inline const vk::UniqueSemaphore &ImageAvailableSemaphore() const
    {
        return m_imageAvailableSemaphore;
    }

    inline const vk::UniqueSemaphore &RenderFinishedSemaphore() const
    {
        return m_renderFinishedSemaphore;
    }

    inline const vk::UniqueFence &InFlightFence() const
    {
        return m_inFlightFence;
    }

  private:
    vk::UniqueCommandBuffer m_commandBuffer;
    vk::UniqueSemaphore m_imageAvailableSemaphore;
    vk::UniqueSemaphore m_renderFinishedSemaphore;
    vk::UniqueFence m_inFlightFence;
};

} // namespace zvk
