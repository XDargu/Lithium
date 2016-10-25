#include "stdafx.h"
#include "MemoryManager.h"
#include <stdlib.h>

cEgMemoryManager::cEgMemoryManager()
{
	mLinearPool = malloc(MEM_SIZE);
	mLinearAllocator = new LinearAllocator(MEM_SIZE, mLinearPool);
	SetCurrentPool(List);
}


cEgMemoryManager::~cEgMemoryManager()
{
	mLinearAllocator->Clear();
	delete mLinearAllocator;
}

inline Allocator & cEgMemoryManager::getAllocator(MemoryPool lPool)
{
	switch (lPool)
	{
	case CurrentPool:
		return getAllocator(mCurrentPool);
		break;
	case List:
		return *mLinearAllocator;
		break;
	}

	return *mLinearAllocator;
}

inline Allocator & cEgMemoryManager::getAllocator()
{
	return getAllocator(mCurrentPool);
}

void*
cEgMemoryManager::Malloc(
	size_t      lSize, 
	sUInt8      lAlignement, 
	MemoryPool  lPool)
{
	assert(lSize != 0);
	void* p = getAllocator(lPool).Allocate(lSize, lAlignement);

	return p;
}

void 
cEgMemoryManager::Free(
	void *      lPointer, 
	MemoryPool  lPool)
{
	getAllocator(lPool).Deallocate(lPointer);
}
