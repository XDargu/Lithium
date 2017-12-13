#ifndef _FIXEDSTRING_H
#define _FIXEDSTRING_H

#include <string.h>

template<sInt32 Size>
class FixedString
{
public:

	FixedString<Size>()
	{
		lString[0] = 0x0;
	}

	FixedString<Size>(const sChar8* arg)
	{
		strncpy_s(lString, arg, Size);
	}

	template<sInt32 S>
	FixedString<Size>(const FixedString<S>& arg)
	{
		assert(S <= Size);
		strncpy_s(lString, arg.lString, Size);
	}

	template<sInt32 S>
	FixedString<Size>(const FixedString<S>&& arg)
	{
		assert(S <= Size);
		strncpy_s(lString, arg.lString, Size);
	}

	sChar8*
	GetString()
	{
		return lString;
	}

    operator sChar8*()
    {
        return GetString();
    }

    const sChar8*
    GetString() const
    {
        return lString;
    }

    operator sChar8*() const
    {
        return GetString();
    }

    operator const sChar8*() const
    {
        return GetString();
    }

	FixedString<Size>& operator=(const sChar8* arg)
	{
		strncpy_s(lString, arg, Size);
		return *this;
	}

	template<sInt32 S>
	FixedString<Size>& operator=(const FixedString<S>& arg)
	{
		assert(S <= Size);
		strncpy_s(lString, arg.lString, Size);
		return *this;
	}

	template<sInt32 S>
	FixedString<Size>& operator=(const FixedString<S>&& arg)
	{
		assert(S <= Size);
		strncpy_s(lString, arg.lString, Size);
		return *this;
	}

	sChar8 lString[Size];
};

typedef FixedString<32> String32;
typedef FixedString<64> String64;
typedef FixedString<128> String128;

typedef const String32&        String32Arg;
typedef const String64&        String64Arg;
typedef const String128&       String128Arg;

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

#endif