#pragma once

#include "stdafx.h"

namespace zvk
{

#ifdef _WIN32

std::wstring UTF8ToWideChar(const char *pChars);

#endif

} // namespace zvk
