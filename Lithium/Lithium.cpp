// Lithium.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <cstdint>
#include <assert.h>
#include <stdio.h>
#include <time.h> 
#include "Core\CoreTypes.h"
#include "Core\Memory\MemoryManager.h"
#include "Core\Memory\Allocator.h"

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

String128 TestFunction(const String64& lTest)
{	
	return String128(lTest);
}

void nada()
{
	int a = 2;
}

int main()
{
	/*String64 a;
	String128 b;

	a = "asdasd";
	b = "asdb";

	b = a;

	String64* g = new String64("ghghgh");

	String128 as = TestFunction(a);

	std::vector < sInt32, STDAllocator<sInt32>> lVector;

	lVector.push_back(2);
	
	int asd = 2;*/
	//sFloat64 timeDiff;

	void* p = new void*();

	LinearAllocator lAlloc(sizeof(sFloat32) * 10000000, p);	

	clock_t timer1 = clock();

	for (int i = 0; i < 10000000; i++)
	{
		sFloat32* f = custom_allocator::AllocateNew<sFloat32>(lAlloc);
	}

	int c = 90000000;

	int &k = c;

	nada();
	
	int* a = new int[c];


	clock_t timer2 = clock();

	for (sInt32 i = 0; i < 10000000; i++)
	{
		sFloat32* f2 = (sFloat32*)malloc(sizeof(sFloat32));
	}

	clock_t timer3 = clock();


	sFloat64 time1 = timer2 - timer1;
	sFloat64 time2 = timer3 - timer2;

	printf("Allocator: %.f\n", time1);
	printf("Malloc: %.f\n", time2);

    return 0;
}

