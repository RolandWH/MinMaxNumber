/* File:    SQLHandler.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Contains functions for connecting to and reading from an SQL server
   License: MIT License (../LICENSE)
*/

#include "SQLHandler.hpp"
#include "mysql.h"
#include "ccolour/colour.h"

#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <cstdlib>


MYSQL* conn;

// Connect to SQl server+database
bool SQLConnect(std::string url, std::string user, std::string pass, std::string db)
{
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, url.c_str(), user.c_str(), pass.c_str(),
        db.c_str(), 3306, NULL, 0))
    {
        // Error handling
        std::stringstream errS;
        errS << "ERROR: " << mysql_error(conn)
            << " (MySQL error code: " << mysql_errno(conn) << ")\n";

        ChangeColour(errS.str().c_str(), RED_FOREGROUND, DEFAULT_COLOR, true);
        return false;
    }

    return true;
}


// Return numbers in a column into vector
std::vector<int64_t> FetchColumns(std::string table, std::string column)
{
    std::vector<int64_t> numList;
    MYSQL_RES* res;
    MYSQL_ROW row;
    std::stringstream queryString;
    queryString << "SELECT " << column << " FROM " << table;

    if (mysql_query(conn, queryString.str().c_str()))
    {
        ChangeColour(
            "ERROR: The names provided for the table and column are not correct\n",
            RED_FOREGROUND,
            DEFAULT_COLOR,
            true
        );
        exit(EXIT_FAILURE);
    }
    res = mysql_store_result(conn);

    while (row = mysql_fetch_row(res))
    {
        numList.push_back(strtoll(row[0], NULL, 10));
    }

    mysql_free_result(res);
    mysql_close(conn);

    return numList;
}
