#pragma once

#include "stdafx.h"

namespace zvk
{

struct QueueFamilyIndices
{
    QueueFamilyIndices(vk::PhysicalDevice device, vk::SurfaceKHR surface);

    bool IsComplete() const;

    std::optional<uint32_t> GraphicsFamily() const;
    std::optional<uint32_t> PresentFamily() const;

    std::vector<uint32_t> UniqueFamilies() const;
    std::vector<uint32_t> UniqueGraphicsAndPresent() const;

  private:
    std::optional<uint32_t> m_graphicsFamily;
    std::optional<uint32_t> m_presentFamily;
};

} // namespace zvk
