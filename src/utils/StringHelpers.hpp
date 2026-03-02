#pragma once
#include <algorithm>
#include <cstdint>
#include <string>

namespace utils
{
    static void replace_all(std::string& s, std::string const& toReplace, std::string const& replaceWith)
    {
        std::string buf;
        size_t pos = 0;
        size_t prevPos;

        // Reserves rough estimate of final size of string.
        buf.reserve(s.size());

        while (true) {
            prevPos = pos;
            pos = s.find(toReplace, pos);
            if (pos == std::string::npos) break;
            buf.append(s, prevPos, pos - prevPos);
            buf += replaceWith;
            pos += toReplace.size();
        }

        buf.append(s, prevPos, s.size() - prevPos);
        s.swap(buf);
    }

    static bool parse_bool(const std::string& str)
    {
        std::string value = str;
        std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return (value == "true" || value == "1");
    }
}