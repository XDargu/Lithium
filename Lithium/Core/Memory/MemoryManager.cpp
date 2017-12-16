#include "stdafx.h"
#include "MemoryManager.h"
#include "Core\Util\PointerMath.h"
#include "LinearAllocator.h"
#include "PoolAllocator.h"
#include "FreeListAllocator.h"
#include "ProxyAllocator.h"
#include <stdlib.h>

cEgMemoryManager::cEgMemoryManager()
{
    //mMainAllocatorMemory = nullptr;
    //mFreeListAllocator = nullptr;
    //mRendererAllocator = nullptr;

    Construct();
}

void
cEgMemoryManager::Construct()
{
    if (!mFreeListAllocator)
    {
        mMainAllocatorMemory = malloc(MEM_SIZE);

        gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Info, cTkDebugConsole::eDebugConsoleMode_Normal, "[Memory Manager] Allocating Main Memory");
        mFreeListAllocator = new (mMainAllocatorMemory) FreeListAllocator(MEM_SIZE - sizeof(FreeListAllocator), pointer_math::Add(mMainAllocatorMemory, sizeof(FreeListAllocator)));

        gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Info, cTkDebugConsole::eDebugConsoleMode_Normal, "[Memory Manager] Allocating Rendering Memory");
        mRendererAllocator = custom_allocator::AllocateNew<ProxyAllocator>(*mFreeListAllocator, *mFreeListAllocator);

        POW2_ASSERT_MSG(mRendererAllocator != nullptr, "Error creating Renderer Allocator");

        SetCurrentPool(List);

        gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Info, cTkDebugConsole::eDebugConsoleMode_Normal, "[Memory Manager] ** Memory Allocation Complete **");
    }
}


cEgMemoryManager::~cEgMemoryManager()
{
    if (mFreeListAllocator != nullptr)
    {
        if (mRendererAllocator != nullptr)
        {
            custom_allocator::DeallocateDelete(*mFreeListAllocator, mRendererAllocator);
        }

        mFreeListAllocator->~FreeListAllocator();
    }
}

inline Allocator & cEgMemoryManager::getAllocator(MemoryPool lPool)
{
    if (!mFreeListAllocator)
    {
        Construct();
    }

	switch (lPool)
	{
	case CurrentPool:
		return getAllocator(mCurrentPool);
		break;
	case List:
        POW2_ASSERT_MSG(mFreeListAllocator, "Invalid allocator");
		return *mFreeListAllocator;
		break;
    case Renderer:
        POW2_ASSERT_MSG(mRendererAllocator, "Invalid allocator");
        return *mRendererAllocator;
        break;
	}

	return *mFreeListAllocator;
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


cEgMemoryManager gMemoryManager;