#pragma once

#include "stdafx.h"

namespace zvk
{

std::vector<const char *> RequiredInstanceExtensions();
bool CheckDeviceExtensionSupport(vk::PhysicalDevice device);

}
