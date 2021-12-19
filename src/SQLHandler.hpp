/* File:    SQLHandler.hpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Contains functions for connecting to and reading from an SQL server
   License: GNU GPLv3 (../LICENSE)
*/
#pragma once

#include <string>
#include <vector>
#include <cstdint>


// Connect to SQl server+database
bool SQLConnect(std::string url, std::string user, std::string pass, std::string db);

// Return numbers in a column into vector
std::vector<int64_t> FetchColumns(std::string table, std::string column);
