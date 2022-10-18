//
// Created by luoli on 2022/10/17.
//

#ifndef COOLCOMPILER_STRINGUTILS_H
#define COOLCOMPILER_STRINGUTILS_H

#include <string>
#include <algorithm>

namespace StringUtils{
    inline bool replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if(start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }

    inline void replaceAll(std::string& str, const std::string& from, const std::string& to) {
        if(from.empty())
            return;
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
        }
    }

    inline std::string toLower(const std::string &str){
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        return result;
    }

    inline std::string toUpper(const std::string &str){
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c){ return std::toupper(c); });

        return result;
    }
}

#endif //COOLCOMPILER_STRINGUTILS_H
