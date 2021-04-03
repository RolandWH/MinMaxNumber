/* File:    StringHandling.h
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Includes functions for handling strings (formating, etc...)
   License: GNU GPLv3 (../LICENSE)
*/
#pragma once

// Convert a string to lowercase
std::string ToLowerString(std::string s);

/* Check whether a string contains only charactors that can be cast to a number
TODO: Find a way to stop user entering a single '-' as it crashes the app */
bool IsStringNumber(std::string s);