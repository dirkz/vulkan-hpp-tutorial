#include "Strings.h"

namespace zvk
{

#ifdef _WIN32

std::wstring UTF8ToWideChar(const char *pChars)
{
    int numChars = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pChars, -1, nullptr, 0);

    if (numChars == 0)
    {
        DWORD errorCode = GetLastError();
        HRESULT hr = HRESULT_FROM_WIN32(errorCode);

        if (SUCCEEDED(hr))
        {
            return L"";
        }
        else
        {
            throw std::runtime_error{
                "win32: could not convert a supposedly UTF-8 string to wide chars"};
        }
    }

    std::wstring result(numChars, L'\0');

    numChars =
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, pChars, -1, result.data(), numChars);

    if (numChars == 0)
    {
        DWORD errorCode = GetLastError();
        HRESULT hr = HRESULT_FROM_WIN32(errorCode);
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();

        throw std::runtime_error{
            "win32: could not convert a supposedly UTF-8 string to wide chars"};
    }

    return result;
}

#endif

} // namespace zvk