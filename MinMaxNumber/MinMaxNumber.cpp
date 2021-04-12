/* File:    MinMaxNumber.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Returns the smallest and biggest number in an SQL column
   License: GNU GPLv3 (../LICENSE) 
*/

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <cxxopts.hpp>
#include <algorithm>
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "SQLHandler.h"


int main(int argc, char** argv) {
    std::string url, user, pass, db, table, column;

    // Check if user has entered enough arguments
    if ((!argv[1]) || (argc < 13 && strcmp(argv[1], "-h") != 0 && strcmp(argv[1], "--help") != 0)) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "ERROR: Not enough arguments" << std::endl;
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "See help (-h, --help) for correct usage";
        return EXIT_FAILURE;
    }

    // Parse command line arguments and options using cxxopts
    try {
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

        if (result.count("help")) {
            std::cout << "MinMaxNumber - Sort numbers in a MySQL table\n"
                << "USAGE:\n\n"
                << "-h, --help\n\tShow this help page\n"
                << "-a, --address\n\tThe ip adress of the desired MySQL server\n"
                << "-u, --user\n\tThe name of a user in your server\n"
                << "-p, --password\n\tThe password of your chosen MySQL user\n"
                << "-d, --database\n\tThe name of a database in your MySQL server\n"
                << "-t, --table\n\tThe name of a table in your MySQL database\n"
                << "-c, --column\n\tThe name of a column in your MySQL table\n";
            return EXIT_SUCCESS;
        }

        url     = result["address"].as<std::string>();
        user    = result["user"].as<std::string>();
        pass    = result["password"].as<std::string>();
        db      = result["database"].as<std::string>();
        table   = result["table"].as<std::string>();
        column  = result["column"].as<std::string>();
    } catch (const cxxopts::OptionException& e) {
        std::cout << "Error parsing options: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    

    // Try and connect to MySQL server using details user provided
    if (!SQLConnect(url, user, pass, db)) {
        return EXIT_FAILURE;
    }
    
    // If connection was sucessful fetch the numbers in the requested column
    std::vector<int> numList = FetchColumns(table, column);


    // Find smallest and biggest numbers in numList
    int smallest = INT_MAX;
    int biggest = NULL;
    for (std::vector<int>::size_type i = 0; i < numList.size(); i++) {
        smallest = std::min(smallest, numList[i]);
        biggest = std::max(biggest, numList[i]);
    }


    // Output results
    std::cout << "The smallest number in your column is ";
    std::cout << smallest << std::endl;
    std::cout << "The biggest number in your column is ";
    std::cout << biggest << std::endl;

    return EXIT_SUCCESS;
}