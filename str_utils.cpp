/*
 * Filename: str_utils.cpp
 * Author: Matt Asnes
 * Date: June 19, 2015
 * Description: Various string utilities used throughout the program.
 *
 */

#include "str_utils.h"

void hex_print(std::string str)
{
        const char *c_str = str.c_str();
        std::cerr << "C++ string as hex: ";
        for (uint i = 0; i < str.length(); i++) {
                std::cerr << std::hex << (int) c_str[i];
        }
        std::cerr << std::endl;
}


std::string replace(std::string str, char ch1, char ch2) 
{
        for (uint i = 0; i < str.length(); i++) {
                if (str[i] == ch1) str[i] = ch2;
        }
        return str;
}

std::string replace_all(std::string subject, const std::string& search,
                                             const std::string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}