#pragma once

#include "stdafx.h"

namespace zvk
{

struct FrameData
{
    FrameData(const vk::Device &device, const vk::CommandPool &pool);

  private:
    vk::UniqueCommandBuffer m_commandBuffer;
    vk::UniqueSemaphore m_imageAvailableSemaphore;
    vk::UniqueSemaphore m_renderFinishedSemaphore;
    vk::UniqueFence m_inFlightFence;
};

} // namespace zvk
