#pragma once

#include "stdafx.h"

namespace zvk
{

struct FrameData
{
    FrameData(const vk::Device &device, const vk::CommandPool &pool);
};

} // namespace zvk
