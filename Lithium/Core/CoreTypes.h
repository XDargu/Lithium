#ifndef _CORE_TYPES_H_
#define _CORE_TYPES_H_

#include "Memory\MemoryManager.h"

// TODO: Override new/delete default c++ operations

cEgMemoryManager gMemoryManager;

template <class T, cEgMemoryManager::MemoryPool Pool = cEgMemoryManager::CurrentPool>
class STDAllocator : public std::allocator<T>
{
public:

	typedef T value_type;
	typedef T *pointer;
	typedef T &reference;
	typedef const T *const_pointer;
	typedef const T &const_reference;
	typedef unsigned size_type;
	typedef unsigned difference_type;

	STDAllocator() {}

	template <class U>
	STDAllocator(STDAllocator<U> const &) {}

	template <class U>
	struct rebind
	{
		typedef STDAllocator<U> other;
	};

	pointer 
	allocate(unsigned n)
	{
		return custom_allocator::AllocateArray<T>(gMemoryManager.getAllocator(Pool), n);
	}

	void 
	deallocate(pointer p, unsigned n)
	{
		custom_allocator::DeallocateArray<T>(gMemoryManager.getAllocator(Pool), p);
	}

	void construct(pointer p, const_reference clone)
	{
		new (p) T(clone);
	}

	void destroy(pointer p)
	{
		p->~T();
	}

	pointer address(reference x) const
	{
		return &x;
	}

	const_pointer address(const_reference x) const
	{
		return &x;
	}

	bool operator==(const STDAllocator &rhs)
	{
		return true;
	}

	bool operator!=(const STDAllocator &rhs)
	{
		return !operator==(rhs);
	}
};

#endif