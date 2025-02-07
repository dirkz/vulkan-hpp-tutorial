#pragma once

#include "stdafx.h"

namespace zvk
{

#ifdef NDEBUG
constexpr bool EnableValidationLayers = false;
#else
constexpr bool EnableValidationLayers = true;
#endif

} // namespace zvk
