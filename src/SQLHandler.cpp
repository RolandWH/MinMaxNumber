/* File:    SQLHandler.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Contains functions for connecting to and reading from an SQL server
   License: MIT License (../LICENSE)
*/

#include "SQLHandler.hpp"
#include "mysql/jdbc.h"
#include "ccolour/colour.h"

#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <cstdlib>


sql::Connection* con;

// Connect to SQl server+database
bool SQLConnect(std::string url, std::string user, std::string pass, std::string db)
{
    try
    {
        sql::Driver* driver = get_driver_instance();
        con = driver->connect(url, user, pass);
        con->setSchema(db);
    } catch (sql::SQLException &err)
    {
        // Error handling
        std::stringstream errS;
        errS << "ERROR: " << err.what();
        errS << " (MySQL error code: " << err.getErrorCode();
        if (err.getSQLState() != "")
        {
            errS << ", SQLState: " << err.getSQLState() << ")";
        }
        else errS << ')';
        
        ChangeColour(errS.str().c_str(), RED_FOREGROUND, DEFAULT_COLOR, true);
        return false;
    }
    return true;
}


// Return numbers in a column into vector
std::vector<int64_t> FetchColumns(std::string table, std::string column)
{
    std::vector<int64_t> numList;
    sql::Statement* stmt;
    sql::ResultSet* res;
    stmt = con->createStatement();
    std::stringstream queryString;
    queryString << "SELECT " << column << " FROM " << table;

    try
    {
        res = stmt->executeQuery(queryString.str());
        while (res->next())
        {
            numList.push_back(res->getInt(1));
        }
        delete res;
    }
    catch (const sql::SQLException&)
    {
        ChangeColour(
            "ERROR: The names provided for the table and column are not correct",
            RED_FOREGROUND,
            DEFAULT_COLOR,
            true
        );
        exit(EXIT_FAILURE);
    }

    delete stmt;
    delete con;
    return numList;
}