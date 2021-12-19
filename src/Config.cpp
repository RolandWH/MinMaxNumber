/* File:    Config.cpp
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Contains functions for manging configuration data
   License: GNU GPLv3 (../LICENSE)
*/

#include "Config.hpp"
#include "INIReader.h"
#include "ccolour/colour.h"

#include <string>
#include <fstream>
#include <iostream>


bool DoesConfigExist(std::string path)
{
    std::ifstream config(path);
    if (config.good()) return true;
    else return false;
}


// TODO: Find a ini library that handles reading AND writing
bool WriteConfig(std::vector<std::string> configData, std::string path)
{
    std::ofstream config(path);
    if (!config.good()) return false;

    config << "[auth]" << '\n'
        << "ip=" << configData[0] << '\n'
        << "user=" << configData[1] << '\n'
        << "pass=" << configData[2] << '\n'

        << "[data]" << '\n'
        << "db=" << configData[3] << '\n'
        << "table=" << configData[4] << '\n'
        << "column=" << configData[5] << '\n';

    return true;
}


std::vector<std::string> ReadConfig(std::string path)
{
    std::vector<std::string> configData;
    INIReader config(path);

    if (config.ParseError() < 0)
    {
        ChangeColour(
            "ERROR: Cannot open configuration file",
            RED_FOREGROUND,
            DEFAULT_COLOR,
            true
        );
        exit(EXIT_FAILURE);
    }

    configData.push_back(config.GetString("auth", "ip", ""));
    configData.push_back(config.GetString("auth", "user", ""));
    configData.push_back(config.GetString("auth", "pass", ""));
    configData.push_back(config.GetString("data", "db", ""));
    configData.push_back(config.GetString("data", "table", ""));
    configData.push_back(config.GetString("data", "column", ""));

    return configData;
}