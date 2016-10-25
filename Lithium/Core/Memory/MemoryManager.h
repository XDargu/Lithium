#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include "Allocator.h"

#define MEM_SIZE 1048576000 //1GB

class cEgMemoryManager
{
public:
	enum MemoryPool
	{
		CurrentPool,
		List,
		Num_Pools
	};

	cEgMemoryManager();

	~cEgMemoryManager();

	Allocator& getAllocator(MemoryPool lPool);
	Allocator& getAllocator();

	void*
	Malloc(
		size_t      lSize, 
		sUInt8      lAlignement, 
		MemoryPool  lPool = CurrentPool );

	void
	Free(
		void*      lPointer, 
		MemoryPool lPool = CurrentPool );

	void inline
	SetCurrentPool(MemoryPool lPool)
	{
		mCurrentPool = lPool;
	}

private:

	MemoryPool mCurrentPool;

public:

	cEgMemoryManager(cEgMemoryManager&) = delete;
	void operator=(cEgMemoryManager const&) = delete;

	static const MemoryPool mDefaultPool = List;

	LinearAllocator*  mLinearAllocator;
	void*             mLinearPool;
};


#endif