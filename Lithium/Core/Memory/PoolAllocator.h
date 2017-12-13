#ifndef _POOL_ALLOCATOR_H
#define _POOL_ALLOCATOR_H

#include "Allocator.h"

class PoolAllocator : public Allocator
{
public:

    PoolAllocator(
        size_t  lObjectSize, 
        sUInt8  lObjectAlignment, 
        size_t  lSize, 
        void*   lMem );

    ~PoolAllocator();

    void* 
    Allocate(
        size_t size, 
        sUInt8 alignment ) override;

    void 
    Deallocate(
        void* lAddress) override;

private:
    PoolAllocator(const PoolAllocator&); //Prevent copies because it might cause errors
    PoolAllocator& operator=(const PoolAllocator&);

    size_t mObjectSize;
    sUInt8 mObjectAlignment;
    void** mFreeList;
};

#endif