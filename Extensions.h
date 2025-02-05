#pragma once

#include "stdafx.h"

namespace zvk
{

std::vector<const char *> RequiredInstanceExtensions();
std::vector<const char *> RequiredDeviceExtensions();

bool CheckDeviceExtensionSupport(vk::PhysicalDevice device);

}
