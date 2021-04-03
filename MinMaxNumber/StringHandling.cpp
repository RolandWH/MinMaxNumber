/* File:    StringHandling.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Includes functions for handling strings (formating, etc...)
   License: GNU GPLv3 (./LICENSE)
*/
#include <iostream>
#include "StringHandling.h"


// Convert a string to lowercase
std::string ToLowerString(std::string s) {
    std::string lower;
    for (std::string::size_type i = 0; i < s.size(); i++) {
        lower.push_back(std::tolower(s[i]));
    }
    return lower;
}


/* Check whether a string contains only charactors that can be cast to a number
TODO: Find a way to stop user entering a single '-' as it crashes the app */
bool IsStringNumber(std::string s) {
    for (std::string::size_type i = 0; i < s.size(); i++) {
        if (!std::isdigit(s[i]) && s[i] != '-') {
            return false;
        }
    }
    return true;
}