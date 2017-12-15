#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_

#include "Allocator.h"
#include <memory>

#define MEM_SIZE 1024ULL * 1024 * 1024 //1GB
//#define MEM_SIZE 128 //1KB

class ProxyAllocator;
class FreeListAllocator;
class PoolAllocator;

class cEgMemoryManager
{
public:
	enum MemoryPool
	{
		CurrentPool,
        List,
        Renderer,
		Num_Pools
	};

	cEgMemoryManager();

	~cEgMemoryManager();

	Allocator& getAllocator(MemoryPool lPool);
	Allocator& getAllocator();

    void
    Construct();

    void
    Destruct();

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

	FreeListAllocator*  mFreeListAllocator;
	void*               mMainAllocatorMemory;

    ProxyAllocator*     mRendererAllocator;
};

extern cEgMemoryManager gMemoryManager;

template <typename T, cEgMemoryManager::MemoryPool Pool>
class CustomAllocator : public std::allocator<T>
{
public:
    typedef size_t size_type;
    typedef T* pointer;
    typedef const T* const_pointer;

    template<typename _Tp1>
    struct rebind
    {
        typedef CustomAllocator<_Tp1, Pool> other;
    };

    pointer allocate(size_type n, const void *hint = 0)
    {
        return custom_allocator::AllocateArray<T>(gMemoryManager.getAllocator(Pool), n);
        //return (T*)gMemoryManager.Malloc(n, __alignof(T), Pool);
    }

    void deallocate(pointer p, size_type n)
    {
        custom_allocator::DeallocateArray<T>(gMemoryManager.getAllocator(Pool), p);
        //gMemoryManager.Free(p, Pool);
    }

    CustomAllocator() throw() : std::allocator<T>() { }
    CustomAllocator(const CustomAllocator &a) throw() : std::allocator<T>(a) { }

    template <class U>
    CustomAllocator(const CustomAllocator<U, Pool> &a) throw() : std::allocator<T>(a) { }
    ~CustomAllocator() throw() { }
};

#endif