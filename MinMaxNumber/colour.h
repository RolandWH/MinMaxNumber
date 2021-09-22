/* File:    colour.h
   Author:  Roland Weir-Howell (r.weirhowell@gmail.com)
   Desc:    Contains function for manipulating colour on ANSI terminals
   License: GNU GPLv3 (../LICENSE)
*/
#pragma once

 #define DEFAULT_COLOR 0
 #define BLACK_FOREGROUND 30
 #define RED_FOREGROUND 31
 #define GREEN_FOREGROUND 32
 #define YELLOW_FOREGROUND 33
 #define BLUE_FOREGROUND 34
 #define PURPLE_FOREGROUND 35
 #define CYAN_FOREGROUND 36
 #define WHITE_FOREGROUND 37
 #define BLACK_BACKGROUND 40
 #define RED_BACKGROUND 41
 #define GREEN_BACKGROUND 42
 #define YELLOW_BACKGROUND 43
 #define BLUE_BACKGROUND 44
 #define PURPLE_BACKGROUND 45
 #define CYAN_BACKGROUND 46
 #define WHITE_BACKGROUND 47

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

#include <stdbool.h>
#include <stdio.h>


/*  Structure of colour change string:
        1. Escape sequence for changing colour (\33)
        2. Opening bracket ( [ )
        3. Foreground colour (%d, fg)
        4. Semicolon (;)
        5. Background colour (%d, bg)
        6. Character to end colour changing (m)
        7. Message passed to function (%s, msg)
        8. Newline (\n)
        If only one colour is being changed then the semicolon is left out

        Example for red text on black background:
        "\33[31;40m"
*/

static void ChangeColour(const char* msg, int bg, int fg, bool revert)
{
    // Enable ANSI support on Windows
    #ifdef _WIN32
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      DWORD dwMode = 0;
      GetConsoleMode(hOut, &dwMode);
      dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      SetConsoleMode(hOut, dwMode);
    #endif // _WIN32

    if (!bg)
    {
        printf("\33[%dm%s\n", fg, msg);
    }
    else if (!fg)
    {
        printf("\33[%dm%s\n", bg, msg);
    }
    else
    {
        printf("\33[%d;%dm%s", fg, bg, msg);
    }

    if (revert) printf("\33[%dm", DEFAULT_COLOR);
}
