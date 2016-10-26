#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

#include "stdafx.h"

class Allocator
{
public:
	Allocator(size_t lSize, void* lStart)
	{
		mSize = lSize;
		mStart = lStart;

		mUsedMemory = 0;
		mNumAllocations = 0;
	}

	virtual ~Allocator()
	{
		assert(mNumAllocations == 0 && mUsedMemory == 0);

		mStart = nullptr;
		mSize = 0;
	}

	virtual void*
	Allocate(size_t mSize, sUInt8 mAlignement = 4) = 0;

	virtual void
	Deallocate(void* mPointer) = 0;

	void* GetStart() const
	{
		return mStart;
	}

	size_t GetSize() const
	{
		return mSize;
	}

	size_t GetUsedMemory() const
	{
		return mUsedMemory;
	}

	size_t GetNumAllocations() const
	{
		return mNumAllocations;
	}

protected:

	inline void*
		AlignForward(void* lAddress, sUInt8 lAlignment)
	{
		// (address + (alignment - 1)) & (~(alignment - 1))
		return (void*)((reinterpret_cast<sUPtr>(lAddress) + static_cast<sUPtr>(lAlignment - 1)) & static_cast<sUPtr>(~(lAlignment - 1)));
	}

	inline sUInt8
		AlignForwardPadding(const void* lAddress, sUInt8 lAlignment)
	{
		sUInt8 lPadding = lAlignment - (reinterpret_cast<sUPtr>(lAddress) & static_cast<sUPtr>(lAlignment - 1));

		if (lPadding == lAlignment)
			return 0; //already aligned

		return lPadding;
	}

	void*	mStart;
	size_t	mSize;

	size_t	mUsedMemory;
	size_t  mNumAllocations;
};

namespace custom_allocator
{
	template<class T>
	T* AllocateNew(Allocator& lAllocator)
	{
		return new (lAllocator.Allocate(sizeof(T), __alignof(T))) T;
	}

	template<class T>
	T* AllocateNew(Allocator& lAllocator, const T& lObject)
	{
		return new (lAllocator.Allocate(sizeof(T), __alignof(T))) T(lObject);
	}

	template<class T>
	void DeallocateDelete(Allocator& lAllocator, T& lObject)
	{
		lObject.~T();
		lAllocator.Deallocate(&lObject);
	}

	template<class T>
	T* AllocateArray(Allocator& lAllocator, size_t lLength)
	{
		assert(lLength != 0);

		sUInt8 lHeaderSize = sizeof(size_t) / sizeof(T);

		if (sizeof(size_t) % sizeof(T) > 0)
		{
			lHeaderSize += 1;
		}

		//Allocate extra space to store array length in the bytes before the array
		T* lpMem = ((T*)lAllocator.Allocate(sizeof(T) * (lLength + lHeaderSize), __alignof(T))) + lHeaderSize;

		*reinterpret_cast<size_t*>((((size_t)lpMem) - 1)) = lLength;

		for (size_t liIndex = 0; liIndex < lLength; liIndex++)
		{
			new (&lpMem[liIndex]) T;
		}

		return lpMem;
	}
	template<class T>
	void DeallocateArray(Allocator& lAllocator, T* lArray)
	{
		assert(lArray != nullptr);

		size_t lLength = (((size_t)lArray) - 1);

		for (size_t liIndex = 0; liIndex < lLength; liIndex++)
		{
			lArray[liIndex].~T();
		}

		//Calculate how much extra memory was allocated to store the length before the array
		sUInt8 lHeaderSize = sizeof(size_t) / sizeof(T);

		if (sizeof(size_t) % sizeof(T) > 0)
		{
			lHeaderSize += 1;
		}

		lAllocator.Deallocate(lArray - lHeaderSize);
	}
}

class LinearAllocator : public Allocator
{
public:

	LinearAllocator(size_t lSize, void* lStart)
		: Allocator(lSize, lStart), mCurrentPos(lStart)
	{
		assert(lSize > 0);
	}

	~LinearAllocator()
	{
		mCurrentPos = nullptr;
	}

	void*
		Allocate(size_t lSize, sUInt8 mAlignement) override
	{
		assert(lSize > 0);

		sUInt8 lPadding = AlignForwardPadding(mCurrentPos, mAlignement);

		if (mUsedMemory + lPadding + lSize > mSize)
		{
			return nullptr;
		}

		sUPtr lAlignedAddress = (sUPtr)mCurrentPos + lPadding;

		mCurrentPos = (void*)(lAlignedAddress + lSize);

		mUsedMemory += lSize + lPadding;
		mNumAllocations++;

		return (void*)lAlignedAddress;
	}

	void
	Deallocate(void* mPointer) override
	{
		//assert(false && "Use clear instead");
	}

	void Clear()
	{
		mNumAllocations = 0;
		mUsedMemory = 0;
		mCurrentPos = mStart;
	}


	LinearAllocator(const LinearAllocator&) = delete;
	LinearAllocator& operator=(const LinearAllocator&) = delete;

	void* mCurrentPos;
};

#endif