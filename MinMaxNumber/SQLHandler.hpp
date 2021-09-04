/* File:    SQLHandler.h
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Contains functions for connecting to and reading from an SQL server
   License: GNU GPLv3 (../LICENSE)
*/
#pragma once

// Connect to SQl server+database
bool SQLConnect(std::string url, std::string user, std::string pass, std::string db);

// Return numbers in a column into vector
std::vector<int> FetchColumns(std::string table, std::string column);
