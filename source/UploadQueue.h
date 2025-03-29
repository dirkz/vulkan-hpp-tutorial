#pragma once

#include "stdafx.h"

#include "MappedBuffer.h"

namespace zvk
{

struct UploadQueue
{
    UploadQueue(const vk::Device device, uint32_t uploadQueue);

    void FinishAndWait();

  private:
    vk::Device m_device;
    vk::UniqueFence m_fence;
    vk::Queue m_queue;
    vk::UniqueCommandPool m_commandPool;
    vk::UniqueCommandBuffer m_commandBuffer;
    std::vector<std::unique_ptr<MappedBuffer>> m_stagingBuffers;
};

} // namespace zvk
