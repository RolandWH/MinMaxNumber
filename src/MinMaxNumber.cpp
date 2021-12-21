/* File:    MinMaxNumber.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Returns the smallest and biggest number in an SQL column
   License: MIT License (../LICENSE)
*/

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "Config.hpp"
#include "ccolour/colour.h"
#include "cxxopts/include/cxxopts.hpp"
#include "SQLHandler.hpp"

#include <filesystem>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

#include <cstring>
#include <cstdlib>
#include <cstdint>

#include <Windows.h>
#include <lmcons.h>


int main(int argc, char** argv)
{
    // Define path for config file and create directory
    namespace fs = std::filesystem;
    DWORD size = UNLEN + 1;
    char username[UNLEN + 1];
    GetUserNameA(username, &size);

    const std::string path = "C:\\Users\\"
        + (std::string)username
        + "\\AppData\\Roaming\\MinMaxNumber\\config.ini";

    fs::create_directory("C:\\Users\\"
        + (std::string)username
        + "\\AppData\\Roaming\\MinMaxNumber");

    std::vector<std::string> configData;
    std::string url, user, pass, db, table, column;
    bool configExists = false, updateConfig = true;

    if (DoesConfigExist(path))
    {
        configData = ReadConfig(path);

        url    = configData[0];
        user   = configData[1];
        pass   = configData[2];
        db     = configData[3];
        table  = configData[4];
        column = configData[5];

        configExists = true;
        updateConfig = false;
    }

    if (!configExists)
    {
        // Terrible code for checking if user has entered enough arguments
        if (!argv[1] || argc < 13 && strcmp(argv[1], "-h") && strcmp(argv[1], "--help"))
        {
            ChangeColour(
                "ERROR: Not enough arguments\n",
                RED_FOREGROUND,
                DEFAULT_COLOR,
                true
            );
            std::cout << "See help (-h, --help) for correct usage\n";
            return EXIT_FAILURE;
        }
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

        if (result.count("address")) url = result["address"].as<std::string>();
        if (result.count("user")) user = result["user"].as<std::string>();
        if (result.count("password")) pass = result["password"].as<std::string>();
        if (result.count("database")) db = result["database"].as<std::string>();
        if (result.count("table")) table = result["table"].as<std::string>();
        if (result.count("column")) column = result["column"].as<std::string>();

        /* Using localhost creates HUGE delay, see:
        https://stackoverflow.com/a/15436435/15568597 */
        if (url == "localhost") url = "127.0.0.1";

        const std::string cmdArgs[6] = {
            "address",
            "user",
            "password",
            "database",
            "table",
            "column"
        };

        if (configExists)
        {
            for (int i = 0; i < 6; i++)
            {
                if (result.count(cmdArgs[i]))
                {
                    if (result[cmdArgs[i]].as<std::string>() != configData[i])
                    {
                        configData[i] = result[cmdArgs[i]].as<std::string>();
                        updateConfig = true;
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < 6; i++)
            {
                configData.push_back(result[cmdArgs[i]].as<std::string>());
            }
        }
    }
    catch (const cxxopts::OptionException& e)
    {
        std::stringstream optErr;
        optErr << "ERROR: " << e.what();
        ChangeColour(optErr.str().c_str(), RED_FOREGROUND, DEFAULT_COLOR, true);
        return EXIT_FAILURE;
    }
    
    // Try and connect to MySQL server using details user provided
    if (!SQLConnect(url, user, pass, db)) return EXIT_FAILURE;

    // If connection was sucessful fetch the numbers in the requested column
    std::vector<int64_t> numList = FetchColumns(table, column);

    // Find smallest and biggest numbers in numList
    int64_t smallest = numList[0];
    int64_t biggest = numList[0];
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
            "WARNING: All outputs are zero, does provided column contain intergers?\n",
            YELLOW_FOREGROUND,
            DEFAULT_COLOR,
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
    
    // Code to remove trailing zeros
    std::string strAvg = std::to_string(avg);
    strAvg = strAvg.substr(0, strAvg.find_last_not_of('0')+1);
    if(strAvg.find('.') == strAvg.size()-1)
    {
        strAvg = strAvg.substr(0, strAvg.size()-1);
    }
    std::cout << strAvg << '\n';

    if (updateConfig)
    {
        std::string choice;
        std::cout << "Do you want to save your settings? [Y/N] ";
        std::cin >> choice;

        if (choice == "Y" || choice == "y")
        {
            if (!WriteConfig(configData, path))
            {
                ChangeColour(
                    "ERROR: Failed to write to configuration file",
                    RED_FOREGROUND,
                    DEFAULT_COLOR,
                    true
                );
            }
        }
    }

    return EXIT_SUCCESS;
}
