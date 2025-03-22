#pragma once

#include "stdafx.h"

#include "UniformBuffer.h"

namespace zvk
{

template <class T> struct FrameData
{
    FrameData(const vk::Device &device, const VmaAllocator allocator, const vk::CommandPool &pool)
        : m_uniformBuffer{allocator, vk::BufferUsageFlags{vk::BufferUsageFlagBits::eUniformBuffer},
                          vk::SharingMode::eExclusive}
    {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo{
            pool, vk::CommandBufferLevel::ePrimary, 1};
        auto commandBuffers = device.allocateCommandBuffersUnique(commandBufferAllocateInfo);
        m_commandBuffer = std::move(commandBuffers[0]);

        vk::SemaphoreCreateInfo semaphoreCreateInfo{};
        m_imageAvailableSemaphore = device.createSemaphoreUnique(semaphoreCreateInfo);
        m_renderFinishedSemaphore = device.createSemaphoreUnique(semaphoreCreateInfo);

        vk::FenceCreateInfo fenceCreateInfo{vk::FenceCreateFlagBits::eSignaled};
        m_inFlightFence = device.createFenceUnique(fenceCreateInfo);
    }

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

    void UpdateUniform(const T &uniform) const
    {
        m_uniformBuffer.Update(uniform);
    }

  private:
    vk::UniqueCommandBuffer m_commandBuffer;
    vk::UniqueSemaphore m_imageAvailableSemaphore;
    vk::UniqueSemaphore m_renderFinishedSemaphore;
    vk::UniqueFence m_inFlightFence;
    UniformBuffer<T> m_uniformBuffer;
};

} // namespace zvk
