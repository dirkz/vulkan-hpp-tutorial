#pragma once

#include "stdafx.h"

namespace zvk
{

struct QueueFamilyIndices
{
    QueueFamilyIndices(const vk::PhysicalDevice device, const vk::SurfaceKHR surface);

    bool IsComplete() const;

    std::optional<uint32_t> GraphicsFamily() const;
    std::optional<uint32_t> PresentFamily() const;
    std::optional<uint32_t> TransferFamily() const;

    std::vector<uint32_t> UniqueFamilies() const;
    std::vector<uint32_t> UniqueGraphicsAndPresent() const;
    std::vector<uint32_t> UniqueGraphicsAndTransfer() const;

  private:
    std::optional<uint32_t> m_graphicsFamily;
    std::optional<uint32_t> m_presentFamily;
    std::optional<uint32_t> m_transferFamily;
};

} // namespace zvk
