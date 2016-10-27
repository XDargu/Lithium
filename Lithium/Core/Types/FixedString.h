#ifndef _FIXEDSTRING_H
#define _FIXEDSTRING_H

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

    const sChar8*
    GetString() const
    {
        return lString;
    }

    operator sChar8*() const
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

#endif