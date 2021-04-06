/* File:    SQLHandler.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Contains functions for connecting to and reading from an SQL server
   License: GNU GPLv3 (../LICENSE)
*/
#include <iostream>
#include <sstream>
#include <vector>
#include <mysql/jdbc.h>
#include "SQLHandler.h"

sql::Connection* con;

// Connect to SQl server+database
bool SQLConnect(std::string url, std::string user, std::string pass, std::string db) {
    try {
        // Try and connect to database
        sql::Driver* driver = get_driver_instance();
        con = driver->connect(url, user, pass);
        con->setSchema(db);
    } catch (sql::SQLException &err) {
        // Error handling
        std::cout << "# ERR: " << err.what();
        std::cout << " (MySQL error code: " << err.getErrorCode();
        std::cout << ", SQLState: " << err.getSQLState() << " )" << std::endl;
        return false;
    }
    return true;
}


// Return numbers in a column into vector
std::vector<int> FetchColumns(std::string table, std::string column) {
    std::vector<int> numList;
    sql::Statement* stmt;
    sql::ResultSet* res;
    stmt = con->createStatement();
    std::stringstream queryString;
    queryString << "SELECT " << column << " FROM " << table;
    res = stmt->executeQuery(queryString.str());
    while (res->next()) {
        numList.push_back(res->getInt(1));
    }

    delete res;
    delete stmt;
    delete con;

    return numList;
}