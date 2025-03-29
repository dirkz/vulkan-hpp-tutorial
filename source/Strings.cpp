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


std::string WideCharToUTF8(const std::wstring &wstring)
{
    size_t len = wstring.length();
    size_t size = len + 1; // optimistic guess
    std::vector<char> utf8chars(size);
    size_t cbWritten =
        WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstring.c_str(), static_cast<int>(len),
                            utf8chars.data(), static_cast<int>(size), NULL, NULL);

    while (cbWritten == 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        size *= 2;
        utf8chars.resize(size);
        cbWritten = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstring.c_str(),
                                        static_cast<int>(len), utf8chars.data(),
                                        static_cast<int>(size), NULL, NULL);
    }

    if (cbWritten == 0)
    {
        auto error_msg_fn = [](DWORD error) {
            switch (error)
            {
            case ERROR_INVALID_FLAGS:
                return std::string{"WideCharToMultiByte ERROR_INVALID_FLAGS"};
            case ERROR_INVALID_PARAMETER:
                return std::string{"WideCharToMultiByte ERROR_INVALID_PARAMETER"};
            case ERROR_NO_UNICODE_TRANSLATION:
                return std::string{"WideCharToMultiByte ERROR_NO_UNICODE_TRANSLATION"};
            default:
                return std::format("WideCharToMultiByte unknown error: {}", error);
            }
        };
        throw std::runtime_error{error_msg_fn(GetLastError())};
    }

    return std::string{utf8chars.data(), cbWritten};
}

#endif

} // namespace zvk