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
	Allocate(size_t mSize, sUInt8 mAlignement = 8) = 0;

	virtual void
	Deallocate(void* lPointer) = 0;

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

	void*	mStart;
	size_t	mSize;

	size_t	mUsedMemory;
	size_t  mNumAllocations;
};

namespace custom_allocator
{

    template <class T, class... Args>
    T* AllocateNew(Allocator& lAllocator, Args&&... args)
    {
        return new (lAllocator.Allocate(sizeof(T), __alignof(T))) T(std::forward<Args>(args)...);
    }

	/*template<class T>
	T* AllocateNew(Allocator& lAllocator, const T& lObject)
	{
		return new (lAllocator.Allocate(sizeof(T), __alignof(T))) T(lObject);
	}*/

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

#endif