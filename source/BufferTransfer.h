#pragma once

#include "stdafx.h"

#include "QueueFamilyIndices.h"

namespace zvk
{

struct BufferTransfer
{
    BufferTransfer(const vk::Device device, const QueueFamilyIndices *familyIndices);

  private:
    vk::UniqueCommandPool m_commandPool;
};

} // namespace zvk
