#pragma once

#include "stdafx.h"

namespace zvk
{

struct QueueFamilyIndices
{
    QueueFamilyIndices(vk::PhysicalDevice device);

    bool IsComplete();

  private:
    std::optional<uint32_t> m_graphicsFamily;
};

} // namespace zvk
