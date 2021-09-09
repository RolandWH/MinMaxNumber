/* File:    SQLHandler.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Contains functions for connecting to and reading from an SQL server
   License: GNU GPLv3 (../LICENSE)
*/

#include "SQLHandler.hpp"
#include "mysql/jdbc.h"
#include "colour.h"

#include <string>
#include <sstream>
#include <vector>


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
        
        ChangeColour(errS.str().c_str(), DEFAULT_COLOR, RED_FOREGROUND, true);
        return false;
    }
    return true;
}


// Return numbers in a column into vector
std::vector<int> FetchColumns(std::string table, std::string column)
{
    std::vector<int> numList;
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
            DEFAULT_COLOR,
            RED_FOREGROUND,
            true
        );
        exit(EXIT_FAILURE);
    }

    delete stmt;
    delete con;
    return numList;
}
