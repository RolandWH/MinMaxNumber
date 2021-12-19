/* File:    Config.hpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Contains functions for manging configuration data
   License: GNU GPLv3 (../LICENSE)
*/
#pragma once

#include <string>
#include <vector>


bool DoesConfigExist(std::string path);
bool WriteConfig(std::vector<std::string> configData, std::string path);
std::vector<std::string> ReadConfig(std::string path);