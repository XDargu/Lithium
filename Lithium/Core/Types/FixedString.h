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

	FixedString<Size>(const sChar* arg)
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

	sChar*
		GetString()
	{
		return lString;
	}

	FixedString<Size>& operator=(const sChar* arg)
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

	sChar lString[Size];
};

typedef FixedString<32> String32;
typedef FixedString<64> String64;
typedef FixedString<128> String128;

#endif