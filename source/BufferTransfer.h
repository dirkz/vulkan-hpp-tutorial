#pragma once

#include "stdafx.h"

#include "MappedBuffer.h"
#include "QueueFamilyIndices.h"
#include "Vma.h"
#include "VmaBuffer.h"

namespace zvk
{

struct BufferTransfer
{
    BufferTransfer(const vk::Device device, const QueueFamilyIndices *familyIndices);

    void Copy(const VmaBuffer &srcBuffer, const VmaBuffer &dstBuffer);

    template <class T>
    VmaBuffer *Upload(const Vma &vma, const std::span<T> contents, vk::BufferUsageFlags usageFlags,
                      const std::span<uint32_t> queues = {})
    {
        auto contentsSize = sizeof(T) * contents.size();

        MappedBuffer *stagingBuffer = vma.NewMappedBuffer(
            contentsSize, vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive);
        memcpy(stagingBuffer->Mapped(), contents.data(), contentsSize);

        usageFlags |= vk::BufferUsageFlagBits::eTransferDst;
        VmaBuffer *deviceLocalBuffer = vma.NewDeviceLocalBuffer(
            contentsSize, usageFlags, vk::SharingMode::eConcurrent, queues);

        std::unique_ptr<MappedBuffer> uniqueBuffer{stagingBuffer};
        m_stagingBuffers.push_back(std::move(uniqueBuffer));

        Copy(*stagingBuffer, *deviceLocalBuffer);

        return deviceLocalBuffer;
    }

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
