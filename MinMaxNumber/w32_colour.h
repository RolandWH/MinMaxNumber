/* File:    w32_colour.h
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Contains function for manipulating console colour on Windows
   License: GNU GPLv3 (../LICENSE)
*/
#pragma once

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GREY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 0xA
#define LIGHT_AQUA 0xB
#define LIGHT_RED 0xC
#define LIGHT_PURPLE 0xD
#define LIGHT_YELLOW 0xE
#define BRIGHT_WHITE 0xF

#include <Windows.h>
#include <stdio.h>


static void ChangeColour(const char* msg, int bg, int fg, bool revert)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hConsole, &info);
    int prevC = info.wAttributes;

    SetConsoleTextAttribute(hConsole, (fg + (bg * 16)));
    printf("%s\n", msg);
    if (revert) SetConsoleTextAttribute(hConsole, prevC);
}
