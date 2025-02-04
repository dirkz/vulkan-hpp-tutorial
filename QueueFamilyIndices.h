#pragma once

#include "stdafx.h"

namespace zvk
{

struct QueueFamilyIndices
{
    QueueFamilyIndices(vk::PhysicalDevice device, vk::SurfaceKHR surface);

    bool IsComplete();

    std::optional<uint32_t> GraphicsFamily();
    std::optional<uint32_t> PresentFamily();

    std::vector<uint32_t> UniqueFamilies();

  private:
    std::optional<uint32_t> m_graphicsFamily;
    std::optional<uint32_t> m_presentFamily;
};

} // namespace zvk
