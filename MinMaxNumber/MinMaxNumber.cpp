/* File:    MinMaxNumber.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Returns the smallest and biggest number in a list 
   License: GNU GPLv3 (../LICENSE) 
*/

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <algorithm>
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "SQLHandler.h"


int main(int argc, char* argv[]) {
    // Parse console arguments
    std::string url;
    std::string user;
    std::string pass;
    std::string db;
    std::string table;
    std::string column;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (argc < 7) {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "ERROR: Not enough arguments" << std::endl;
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "Correct argument order is: ";
        std::cout << "<url> <username> <password> <database> <table> <column>";
        return EXIT_FAILURE;
    } else {
        url     = argv[1];
        user    = argv[2];
        pass    = argv[3];
        db      = argv[4];
        table   = argv[5];
        column  = argv[6];
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

    // In Windows, pause command gives user a chance to read results and quit
    system("pause");
    return EXIT_SUCCESS;
}