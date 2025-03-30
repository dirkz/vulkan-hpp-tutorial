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

    template <class T>
    VmaBuffer *UploadData(const std::span<T> contents, vk::BufferUsageFlags usageFlags)
    {
        auto contentsSize = sizeof(T) * contents.size();

        MappedBuffer *stagingBuffer = m_vma.NewMappedBuffer(
            contentsSize, vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive);
        memcpy(stagingBuffer->Mapped(), contents.data(), contentsSize);

        usageFlags |= vk::BufferUsageFlagBits::eTransferDst;
        VmaBuffer *deviceLocalBuffer =
            m_vma.NewDeviceLocalBuffer(contentsSize, usageFlags, vk::SharingMode::eExclusive);

        std::unique_ptr<MappedBuffer> uniqueBuffer{stagingBuffer};
        m_stagingBuffers.push_back(std::move(uniqueBuffer));

        vk::BufferCopy copyRegion{0, 0, stagingBuffer->Size()};
        m_commandBuffer->copyBuffer(stagingBuffer->Buffer(), deviceLocalBuffer->Buffer(),
                                    {copyRegion});

        return deviceLocalBuffer;
    }

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
