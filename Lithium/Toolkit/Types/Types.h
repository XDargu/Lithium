#ifndef _TKTYPES_H_
#define _TKTYPES_H_

#include <vector>
#include <map>
#include "Core\Memory\MemoryManager.h"

typedef FixedString<32> cTkString32;
typedef FixedString<64> cTkString64;
typedef FixedString<128> cTkString128;

typedef const cTkString32&        cTkString32Arg;
typedef const cTkString64&        cTkString64Arg;
typedef const cTkString128&       cTkString128Arg;

namespace cTkString
{
    sInt32
    GetLength(const sChar8* arg) { return (sInt32)strlen(arg); }

    sBool
    IsEmpty(const sChar8* arg) { return arg != NULL && arg[0] == '\0'; }

    sBool
    Contains(const sChar8* arg, const sChar8* find) { return strstr(arg, find) != nullptr; }

    sInt32 inline
    Find(const sChar8* arg, const sChar8* find)
    {
        const sChar8* lPointer = strstr(arg, find);
        if (lPointer)
        {
            return (sInt32)(lPointer - arg);
        }

        return 0;
    }

    sBool
    IsEqual(const sChar8* arg1, const sChar8* arg2) { return strcmp(arg1, arg2) == 0; }
}

template <class T, cEgMemoryManager::MemoryPool Pool = cEgMemoryManager::CurrentPool>
using TkVector = std::vector<T, CustomAllocator<T, Pool>>;

template <class Key, class T, cEgMemoryManager::MemoryPool Pool = cEgMemoryManager::CurrentPool>
using TkMap = std::map<Key, T, std::less<Key>, CustomAllocator<T, Pool>>;



#endif