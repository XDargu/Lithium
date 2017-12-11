#include "stdafx.h"
#include "DebugConsole.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

namespace ConsoleForeground
{
    enum {
        BLACK       = 0,
        DARKBLUE    = FOREGROUND_BLUE,
        DARKGREEN   = FOREGROUND_GREEN,
        DARKCYAN    = FOREGROUND_GREEN | FOREGROUND_BLUE,
        DARKRED     = FOREGROUND_RED,
        DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
        DARKYELLOW  = FOREGROUND_RED | FOREGROUND_GREEN,
        DARKGRAY    = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        GRAY        = FOREGROUND_INTENSITY,
        BLUE        = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
        GREEN       = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
        CYAN        = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
        RED         = FOREGROUND_INTENSITY | FOREGROUND_RED,
        MAGENTA     = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
        YELLOW      = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
        WHITE       = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    };
}

namespace ConsoleBackground
{
    enum {
        BLACK       = 0,
        DARKBLUE    = BACKGROUND_BLUE,
        DARKGREEN   = BACKGROUND_GREEN,
        DARKCYAN    = BACKGROUND_GREEN | BACKGROUND_BLUE,
        DARKRED     = BACKGROUND_RED,
        DARKMAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
        DARKYELLOW  = BACKGROUND_RED | BACKGROUND_GREEN,
        DARKGRAY    = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
        GRAY        = BACKGROUND_INTENSITY,
        BLUE        = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
        GREEN       = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
        CYAN        = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
        RED         = BACKGROUND_INTENSITY | BACKGROUND_RED,
        MAGENTA     = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
        YELLOW      = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
        WHITE       = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
    };
}

namespace ConsoleTypeColour
{
    enum {
        TYPE_INFO    = ConsoleForeground::WHITE | ConsoleBackground::DARKBLUE,
        TYPE_WARNING = ConsoleForeground::YELLOW | ConsoleBackground::DARKBLUE,
        TYPE_ERROR   = ConsoleForeground::RED | ConsoleBackground::DARKBLUE
    };
}

const int ConsoleColours[3] = {
    ConsoleTypeColour::TYPE_INFO,
    ConsoleTypeColour::TYPE_WARNING,
    ConsoleTypeColour::TYPE_ERROR,
};

HANDLE mConsoleHandle;

void
cTkDebugConsole::Construct()
{

    meConsoleMode = eDebugConsoleMode_Verbose;

    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Set colours
    CONSOLE_SCREEN_BUFFER_INFOEX cbi;
    cbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    GetConsoleScreenBufferInfoEx(mConsoleHandle, &cbi);
    cbi.wAttributes = ConsoleForeground::WHITE | ConsoleBackground::DARKBLUE;
    SetConsoleScreenBufferInfoEx(mConsoleHandle, &cbi);

    SetConsoleTextAttribute(mConsoleHandle, ConsoleTypeColour::TYPE_INFO);

    SetConsoleSize(150, 60);
}

void
cTkDebugConsole::Write(
    eDebugConsoleType   leType, 
    eDebugConsoleMode   leMode,
    const char*         lText,
    ... )
{
    if (meConsoleMode > leMode)
        return;

    va_list lList;

    va_start(lList, lText);
    char lBuffer[512];
    size_t lSize = vsnprintf(lBuffer, sizeof(lBuffer) - 1, lText, lList);
    va_end(lList);

    wchar_t wtext[512];
    mbstowcs(wtext, lBuffer, lSize + 1); //Plus null
    LPWSTR ptr = wtext;

    SetConsoleTextAttribute(mConsoleHandle, ConsoleColours[leType]);

    WriteConsole(mConsoleHandle, ptr, (DWORD)lSize, NULL, NULL);
    WriteConsole(mConsoleHandle, "\n", 1, NULL, NULL);
    
}

void cTkDebugConsole::SetConsoleSize(
    sInt16 liWidth, 
    sInt16 liHeight )
{
    // If either dimension is greater than the largest console window we can have,
    // there is no point in attempting the change.
    {
        COORD largestSize = GetLargestConsoleWindowSize(mConsoleHandle);
        if (liWidth > largestSize.X)
        {
            Write(eDebugConsoleType_Error, eDebugConsoleMode_Normal, "[Debug Console] Error resizing: width too large");
        }
        if (liHeight > largestSize.Y)
        {
            Write(eDebugConsoleType_Error, eDebugConsoleMode_Normal, "[Debug Console] Error resizing: height too large");
        }
    }


    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(mConsoleHandle, &bufferInfo))
    {
        // Error, can't get buffer info
    }

    SMALL_RECT& winInfo = bufferInfo.srWindow;
    COORD windowSize = { winInfo.Right - winInfo.Left + 1, winInfo.Bottom - winInfo.Top + 1 };

    if (windowSize.X > liWidth || windowSize.Y > liHeight)
    {
        // window size needs to be adjusted before the buffer size can be reduced.
        SMALL_RECT info =
        {
            0,
            0,
            liWidth < windowSize.X ? liWidth - 1 : windowSize.X - 1,
            liHeight < windowSize.Y ? liHeight - 1 : windowSize.Y - 1
        };

        if (!SetConsoleWindowInfo(mConsoleHandle, TRUE, &info))
        {
            Write(eDebugConsoleType_Error, eDebugConsoleMode_Normal, "[Debug Console] Error resizing: unable to resize window without resizing buffer");
        }
    }

    COORD size = { liWidth, liHeight };
    if (!SetConsoleScreenBufferSize(mConsoleHandle, size))
    {
        Write(eDebugConsoleType_Error, eDebugConsoleMode_Normal, "[Debug Console] Error resizing: unable to resize screen buffer");
    }

    SMALL_RECT info = { 0, 0, liWidth - 1, liHeight - 1 };
    if (!SetConsoleWindowInfo(mConsoleHandle, TRUE, &info))
    {
        Write(eDebugConsoleType_Error, eDebugConsoleMode_Normal, "[Debug Console] Error resizing: unable to resize window after resizing buffer");
    }
}

cTkDebugConsole gDebugConsole;