#ifndef _DEBUG_CONSOLE_H
#define _DEBUG_CONSOLE_H

class cTkDebugConsole
{
public:

    enum eDebugConsoleType
    {
        eDebugConsoleType_Info,
        eDebugConsoleType_Warning,
        eDebugConsoleType_Error
    };

    enum eDebugConsoleMode
    {
        eDebugConsoleMode_Verbose   = 0,
        eDebugConsoleMode_Normal    = 1,
        eDebugConsoleMode_Critical  = 2
    };

    eDebugConsoleMode   meConsoleMode;

    void
    Construct();

    void
    Write(
        eDebugConsoleType   leType, 
        eDebugConsoleMode   leMode,
        const char*         lText,
        ...
    );

    void
    WriteSimpleInfo(const char* lText)
    {
        Write(eDebugConsoleType_Info, eDebugConsoleMode_Normal, lText);
    }

    void
    SetMinMessageLevel(eDebugConsoleMode eMessageLevel)
    {
        meConsoleMode = eMessageLevel;
    }

private:

    void
    SetConsoleSize(
        sInt16 liWidth, 
        sInt16 liHeight
    );
};

extern cTkDebugConsole gDebugConsole;

#endif