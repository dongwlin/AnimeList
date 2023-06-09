#pragma once

#include <Windows.h>
#include <string>
#include <xstring>
#include <utility>
#include <iomanip>
#include <chrono>

namespace utils
{

    std::string UTF8ToGBK(const std::string& strUTF8)
    {
        int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
        TCHAR* wszGBK = new TCHAR[len + 1];
        memset(wszGBK, 0, len * 2 + 2);
        MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);

        len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
        char* szGBK = new char[len + 1];
        memset(szGBK, 0, len + 1);
        WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
        std::string strTemp(szGBK);
        delete[]szGBK;
        delete[]wszGBK;
        return strTemp;
    }

    std::string GBKToUTF8(const std::string& strGBK)
    {
        int len = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
        TCHAR* wszUTF8 = new TCHAR[len + 1];
        memset(wszUTF8, 0, len * 2 + 2);
        MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, wszUTF8, len);

        len = WideCharToMultiByte(CP_UTF8, 0, wszUTF8, -1, NULL, 0, NULL, NULL);
        char* szUTF8 = new char[len + 1];
        memset(szUTF8, 0, len + 1);
        WideCharToMultiByte(CP_UTF8, 0, wszUTF8, -1, szUTF8, len, NULL, NULL);
        std::string strTemp(szUTF8);
        delete[]szUTF8;
        delete[]wszUTF8;
        return strTemp;
    }

    std::wstring stringToWString_ansi(const std::string& str)
    {
        size_t strLen = str.size();
        int num = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
        wchar_t* wide = new wchar_t[num];
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wide, num);
        std::wstring w_str(wide);
        delete[] wide;
        return w_str;
    }

    std::wstring stringToWString_utf8(const std::string& str)
    {
        size_t strLen = str.size();
        int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        wchar_t* wide = new wchar_t[num];
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
        std::wstring w_str(wide);
        delete[] wide;
        return w_str;
    }

    enum CodeType 
    {
        ANSI = 0,
        UTF8
    };

    void open(std::string folderName, CodeType codeType = ANSI)
    {
        std::wstring tmp;
        switch (codeType)
        {
        case ANSI:
            tmp = stringToWString_ansi(folderName);
            break;
        case UTF8:
            tmp = stringToWString_utf8(folderName);
            break;
        default:
            break;
        }

        if (tmp.empty())
        {
            std::cout << "Error: Code Type not exist." << std::endl;
            exit(1);
        }

        ShellExecute(nullptr, L"open", tmp.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    }

    std::string url_decode(const std::string& str) {
        std::string result;
        result.reserve(str.size());
        const char* cur = str.c_str();
        const char* end = cur + str.size();
        while (cur < end) {
            if (*cur == '+') {
                result.push_back(' ');
                cur++;
            }
            else if (*cur == '%' && (end - cur) > 2 && std::isxdigit(*(cur + 1)) && std::isxdigit(*(cur + 2))) {
                char c = static_cast<char>(std::stoi(std::string(cur + 1, cur + 3), nullptr, 16));
                result.push_back(c);
                cur += 3;
            }
            else {
                result.push_back(*cur);
                cur++;
            }
        }
        return result;
    }

    bool to_bool(std::string str)
    {
        if (str == "true" || str == "1")
        {
            return true;
        }

        return false;
    }

    std::string getTimestamp()
    {
        auto ms = std::chrono::steady_clock::now().time_since_epoch();
        return std::to_string(ms.count());
    }
}
