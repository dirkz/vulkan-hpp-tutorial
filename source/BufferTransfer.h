#pragma once

#include "stdafx.h"

#include "QueueFamilyIndices.h"
#include "VmaBuffer.h"

namespace zvk
{

struct BufferTransfer
{
    BufferTransfer(const vk::Device device, const QueueFamilyIndices *familyIndices);
    void Copy(const VmaBuffer &srcBuffer, const VmaBuffer &dstBuffer);
    void FinishAndWait();

  private:
    vk::Device m_device;
    vk::UniqueFence m_fence;
    vk::Queue m_queue;
    vk::UniqueCommandPool m_commandPool;
    vk::UniqueCommandBuffer m_commandBuffer;
};

} // namespace zvk
