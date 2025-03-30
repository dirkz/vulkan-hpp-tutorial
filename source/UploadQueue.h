#pragma once

#include "stdafx.h"

#include "MappedBuffer.h"
#include "Vma.h"
#include "VmaImage.h"

namespace zvk
{

struct UploadQueue
{
    UploadQueue(const Vma &vma, const vk::Device device, uint32_t uploadQueue);

    VmaImage UploadImage(int width, int height, int size, void *pImageData);

    void FinishAndWait();

  private:
    const Vma &m_vma;
    vk::Device m_device;
    vk::UniqueFence m_fence;
    vk::Queue m_uploadQueue;
    vk::UniqueCommandPool m_commandPool;
    vk::UniqueCommandBuffer m_commandBuffer;
    std::vector<std::unique_ptr<MappedBuffer>> m_stagingBuffers;
};

} // namespace zvk
