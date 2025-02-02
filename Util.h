#pragma once

#include "stdafx.h"

namespace vkz
{

/// <summary>
/// A temporary vector of char pointers.
/// </summary>
/// <param name="strings">A vector of strings</param>
/// <returns>A temporary vector of char pointers that is only valid while the original
/// <code>strings</code> is not mutated or goes out of scope</returns>
std::vector<const char *> TmpCharPointers(std::vector<std::string> strings);

} // namespace vkz
