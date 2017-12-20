#include "stdafx.h"
#include "Asserter.h"

#include <cstdio>
#include <cstdarg>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "Resources\resource.h"
#include "StackWalker.h"

struct IngoreTableEntry
{
    const char* macFile;
    const int   macLine;

    IngoreTableEntry(
        const char* file,
        const int line) : macFile(file), macLine(line)
    {
    }

    bool operator==(const IngoreTableEntry& lOther) 
    {
        return (macLine == lOther.macLine) && !strcmp(macFile, lOther.macFile);
    }

    bool operator!=(const IngoreTableEntry& lOther) {
        return this == &lOther;
    }
};

std::vector<IngoreTableEntry> maIgnoreTable;

struct AssertParam
{
    const char* macMessage;
    const char* macFile;
    const int macLine;
    sBool mbIgnore;
    sBool mbIgnoreAlways;

    AssertParam(
        const char* msg,
        const char* file,
        const int line) : macMessage(msg), macFile(file), macLine(line)
    {
        mbIgnore = FALSE;
        mbIgnoreAlways = FALSE;
    }

    std::string GetText()
    {
        std::string mText8 = "ASSERT: " + (macMessage ? std::string(macMessage) : "") + "\r\nLine: " + std::to_string(macLine) + "\r\nFile: " + std::string(macFile);
        return mText8;
    }

    std::string GetCallstackText()
    {
        std::string mText8 = "ASSERT: " + (macMessage ? std::string(macMessage) : "") + "\r\nLine: " + std::to_string(macLine) + "\r\nFile: " + std::string(macFile);
        return mText8;
    }

    std::wstring GetWText()
    {
        std::string mText8 = GetText();

        std::wstring mText = std::wstring(mText8.length(), L' '); // Make room for characters
        std::copy(mText8.begin(), mText8.end(), mText.begin());

        return mText;
    }

    std::wstring GetWCallstackText()
    {
        std::string mText8 = GetCallstackText();

        std::wstring mText = std::wstring(mText8.length(), L' '); // Make room for characters
        std::copy(mText8.begin(), mText8.end(), mText.begin());

        return mText;
    }
};

INT_PTR CALLBACK DialogProc(
    HWND hwnd,
    UINT message,
    WPARAM wp,
    LPARAM lp)
{
    static AssertParam* lpAssertParam = nullptr;
    switch (message)
    {
    case WM_INITDIALOG:
    {
        lpAssertParam = (AssertParam*)lp;
        SetDlgItemText(hwnd, IDC_EDIT1, lpAssertParam->GetWText().c_str());

        return TRUE;
    }
    case WM_COMMAND:
    {
        int ctl = LOWORD(wp);
        int event = HIWORD(wp);

        if (ctl == IDIGNORE && event == BN_CLICKED)
        {
            lpAssertParam->mbIgnore = TRUE;
            DestroyWindow(hwnd);
            return TRUE;
        }
        else if (ctl == IDIGNOREALL && event == BN_CLICKED)
        {
            lpAssertParam->mbIgnore = TRUE;
            lpAssertParam->mbIgnoreAlways = TRUE;
            DestroyWindow(hwnd);
            return TRUE;
        }
        else if (ctl == IDBREAK && event == BN_CLICKED)
        {
            DestroyWindow(hwnd);
            return TRUE;
        }
        return FALSE;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return TRUE;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return TRUE;
    }

    return FALSE;
}

namespace xassert
{
    namespace
    {    
        Assert::FailBehavior DefaultHandler(const char* condition,
            const char* msg,
            const char* file,
            const int line)
        {
            IngoreTableEntry lEntry(file, line);

            auto it = std::find(maIgnoreTable.begin(), maIgnoreTable.end(), lEntry);
            sBool lbInTable = it != maIgnoreTable.end();

            if (lbInTable)
            {
                return Assert::Continue;
            }
            else
            {

                AssertParam lAssertParam(msg, file, line);
                LPARAM lParam = (LPARAM)&lAssertParam;

                gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Error, cTkDebugConsole::eDebugConsoleMode_Critical, lAssertParam.GetText().c_str());

                //stack_trace();

                HWND hDlg = CreateDialogParamW(
                    GetModuleHandle(0),
                    MAKEINTRESOURCE(IDASSERT),
                    0,
                    DialogProc,
                    lParam
                );

                RECT rc;

                GetWindowRect(hDlg, &rc);
                sInt32 xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
                sInt32 yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

                SetWindowPos(hDlg, HWND_TOPMOST, xPos, yPos, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
                ShowWindow(hDlg, SW_SHOW);

                MSG lMessage;
                while (GetMessage(&lMessage, 0, 0, 0)) {
                    if (!IsDialogMessage(hDlg, &lMessage)) {
                        TranslateMessage(&lMessage);
                        DispatchMessage(&lMessage);
                    }
                }

                /*std::printf("%s(%d): Assert Failure: ", file, line);

                if (condition != NULL)
                    std::printf("'%s' ", condition);

                if (msg != NULL)
                    std::printf("%s", msg);

                std::printf("\n");*/

                if (lAssertParam.mbIgnoreAlways)
                {
                    maIgnoreTable.push_back(lEntry);
                }

                return lAssertParam.mbIgnore ? Assert::Continue : Assert::Halt;
            }
        }

        Assert::Handler& GetAssertHandlerInstance()
        {
            static Assert::Handler s_handler = &DefaultHandler;
            return s_handler;
        }
    }

    Assert::Handler Assert::GetHandler()
    {
        return GetAssertHandlerInstance();
    }

    void Assert::SetHandler(Assert::Handler newHandler)
    {
        GetAssertHandlerInstance() = newHandler;
    }

    Assert::FailBehavior Assert::ReportFailure(const char* condition,
        const char* file,
        const int line,
        const char* msg, ...)
    {
        const char* message = NULL;
        if (msg != NULL)
        {
            char messageBuffer[1024];
            {
                va_list args;
                va_start(args, msg);
                vsnprintf(messageBuffer, 1024, msg, args);
                va_end(args);
            }

            message = messageBuffer;
        }

        return GetAssertHandlerInstance()(condition, message, file, line);
    }
}