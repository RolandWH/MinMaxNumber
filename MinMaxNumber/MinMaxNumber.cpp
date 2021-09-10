/* File:    MinMaxNumber.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Returns the smallest and biggest number in an SQL column
   License: GNU GPLv3 (../LICENSE) 
*/

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "colour.h"
#include "cxxopts.hpp"
#include "SQLHandler.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>

#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <climits>
#include <cstddef>


int main(int argc, char** argv)
{
    std::string url, user, pass, db, table, column;
 
    // Terrible code for checking if user has entered enough arguments
    if (!argv[1] || argc < 13 && strcmp(argv[1], "-h") != 0 && strcmp(argv[1], "--help") != 0)
    {
        ChangeColour(
            "ERROR: Not enough arguments",
            DEFAULT_COLOR,
            RED_FOREGROUND,
            true
        );
        std::cout << "See help (-h, --help) for correct usage\n";
        return EXIT_FAILURE;
    }

    // Parse command line arguments and options using cxxopts
    try
    {
        cxxopts::Options options("MinMaxNumber", "Sort numbers in a MySQl table");
        options.add_options()
            ("h,help", "Show help page")
            ("a,address", "MySQl server address", cxxopts::value<std::string>())
            ("u,user", "MySQL server username", cxxopts::value<std::string>())
            ("p,password", "MySQL user password", cxxopts::value<std::string>())
            ("d,database", "MySQL database name", cxxopts::value<std::string>())
            ("t,table", "MySQL table name", cxxopts::value<std::string>())
            ("c,column", "MySQL table column name", cxxopts::value<std::string>());
        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            std::cout << "MinMaxNumber - Sort numbers in a MySQL table\n"
                << "USAGE:\n\n"
                << "-h, --help\n\tShow this help page\n"
                << "-a, --address\n\tThe ip address of the desired SQL server\n"
                << "-u, --user\n\tThe name of a user in your server\n"
                << "-p, --password\n\tThe password of your chosen SQL user\n"
                << "-d, --database\n\tThe name of a database in a SQL server\n"
                << "-t, --table\n\tThe name of a table in a SQL database\n"
                << "-c, --column\n\tThe name of a column in a SQL table\n";
            return EXIT_SUCCESS;
        }

        url     = result["address"].as<std::string>();
        user    = result["user"].as<std::string>();
        pass    = result["password"].as<std::string>();
        db      = result["database"].as<std::string>();
        table   = result["table"].as<std::string>();
        column  = result["column"].as<std::string>();
    } catch (const cxxopts::OptionException& e)
    {
        std::stringstream optErr;
        optErr << "ERROR: " << e.what();
        ChangeColour(optErr.str().c_str(), DEFAULT_COLOR, RED_FOREGROUND, true);
        return EXIT_FAILURE;
    }
    
    // Try and connect to MySQL server using details user provided
    if (!SQLConnect(url, user, pass, db)) return EXIT_FAILURE;

    // If connection was sucessful fetch the numbers in the requested column
    std::vector<int64_t> numList = FetchColumns(table, column);

    // Find smallest and biggest numbers in numList
    int64_t smallest = INT64_MAX;
    int64_t biggest = NULL;
    int64_t sum = NULL;
    for (std::vector<int64_t>::size_type i = 0; i < numList.size(); i++)
    {
        smallest = std::min(smallest, numList[i]);
        biggest = std::max(biggest, numList[i]);
        sum += numList[i];
    }
    double avg = (double)sum / numList.size();

    // Output results
    if (!smallest && !biggest && !avg && !sum)
    {
        ChangeColour(
            "WARNING: All outputs are zero, does provided column contain intergers?",
            DEFAULT_COLOR,
            YELLOW_FOREGROUND,
            true
        );
    }
    std::cout << "The smallest number in your column is ";
    std::cout << smallest << '\n';
    std::cout << "The biggest number in your column is ";
    std::cout << biggest << '\n';
    std::cout << "The total of all the numbers in your column is ";
    std::cout << sum << '\n';
    std::cout << "The average of all the numbers in your column is ";
    
    std::string strAvg = std::to_string(avg);
    strAvg = strAvg.substr(0, strAvg.find_last_not_of('0')+1);
    if(strAvg.find('.') == strAvg.size()-1)
    {
        strAvg = strAvg.substr(0, strAvg.size()-1);
    }
    std::cout << strAvg << '\n';

    return EXIT_SUCCESS;
}
