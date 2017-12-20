#ifndef _STACK_WALKER_H
#define _STACK_WALKER_H

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib" )


static void stack_trace(void) {

    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    CONTEXT context;
    memset(&context, 0, sizeof(CONTEXT));
    context.ContextFlags = CONTEXT_FULL;
    RtlCaptureContext(&context);

    SymInitialize(process, NULL, TRUE);

    DWORD image;
    STACKFRAME64 stackframe;
    ZeroMemory(&stackframe, sizeof(STACKFRAME64));

#ifdef _M_IX86
    image = IMAGE_FILE_MACHINE_I386;
    stackframe.AddrPC.Offset = context.Eip;
    stackframe.AddrPC.Mode = AddrModeFlat;
    stackframe.AddrFrame.Offset = context.Ebp;
    stackframe.AddrFrame.Mode = AddrModeFlat;
    stackframe.AddrStack.Offset = context.Esp;
    stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
    image = IMAGE_FILE_MACHINE_AMD64;
    stackframe.AddrPC.Offset = context.Rip;
    stackframe.AddrPC.Mode = AddrModeFlat;
    stackframe.AddrFrame.Offset = context.Rsp;
    stackframe.AddrFrame.Mode = AddrModeFlat;
    stackframe.AddrStack.Offset = context.Rsp;
    stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
    image = IMAGE_FILE_MACHINE_IA64;
    stackframe.AddrPC.Offset = context.StIIP;
    stackframe.AddrPC.Mode = AddrModeFlat;
    stackframe.AddrFrame.Offset = context.IntSp;
    stackframe.AddrFrame.Mode = AddrModeFlat;
    stackframe.AddrBStore.Offset = context.RsBSP;
    stackframe.AddrBStore.Mode = AddrModeFlat;
    stackframe.AddrStack.Offset = context.IntSp;
    stackframe.AddrStack.Mode = AddrModeFlat;
#endif

    for (int i = 0; i < 25; i++) {
        StackWalk64(
            image, process, thread,
            &stackframe, &context, NULL,
            SymFunctionTableAccess64, SymGetModuleBase64, NULL);
        
        BOOL result = StackWalk64(
            image, process, thread,
            &stackframe, &context, NULL,
            SymFunctionTableAccess64, SymGetModuleBase64, NULL);

        if (!result) { break; }

        char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        DWORD64 displacement = 0;

        if (SymFromAddr(process, stackframe.AddrPC.Offset, &displacement, symbol)) {
            char buf[1024];
            sprintf(buf, "[%i] %s", i, symbol->Name);
            gDebugConsole.WriteSimpleInfo(buf);
            OutputDebugStringA(buf);
        }
        else {
            char buf[1024];            
            sprintf(buf, "[%i] ???", i);
            gDebugConsole.WriteSimpleInfo(buf);

            DWORD a = GetLastError();

            OutputDebugStringA(buf);
        }

    }

    SymCleanup(process);

}

#endif