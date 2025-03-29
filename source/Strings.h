#pragma once

#include "stdafx.h"

namespace zvk
{

#ifdef _WIN32

std::wstring UTF8ToWideChar(const char *pChars);
std::string WideCharToUTF8(const std::wstring &wstring);

#endif

} // namespace zvk
