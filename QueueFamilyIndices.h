#pragma once

#include "stdafx.h"

namespace zvk
{

struct QueueFamilyIndices
{
    QueueFamilyIndices(vk::PhysicalDevice device);

    bool IsComplete();

    std::optional<uint32_t> GraphicsFamiliy();

  private:
    std::optional<uint32_t> m_graphicsFamily;
};

} // namespace zvk
