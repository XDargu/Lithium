#include "stdafx.h"

#include "PoolAllocator.h"
#include "Core\Util\PointerMath.h"

PoolAllocator::PoolAllocator(
    size_t  lObjectSize, 
    sUInt8  lObjectAlignment, 
    size_t  lSize, 
    void*   lMem)
    : Allocator(lSize, lMem), 
    mObjectSize(lObjectSize), 
    mObjectAlignment(lObjectAlignment)
{
    assert(lObjectSize >= sizeof(void*));

    //Calculate adjustment needed to keep object correctly aligned
    sUInt8 lAdjustment = pointer_math::AlignForwardAdjustment(lMem, lObjectAlignment);
    mFreeList = (void**)pointer_math::Add(lMem, lAdjustment);
    size_t numObjects = (lSize - lAdjustment) / lObjectSize;
    void** p = mFreeList;

    //Initialize free blocks list
    for (size_t i = 0; i < numObjects - 1; i++)
    {
        *p = pointer_math::Add(p, lObjectSize);
        p = (void**)*p;
    }
    *p = nullptr;
}
PoolAllocator::~PoolAllocator()
{
    mFreeList = nullptr;
}

void* 
PoolAllocator::Allocate(
    size_t  lSize, 
    sUInt8  lAlignment)
{
    assert(lSize == mObjectSize && lAlignment == mObjectAlignment);

    if (mFreeList == nullptr)
        return nullptr;

    void* p = mFreeList;
    mFreeList = (void**)(*mFreeList);
    mUsedMemory += lSize;
    mNumAllocations++;

    return p;
}
void 
PoolAllocator::Deallocate(
    void* lAddress )
{
    *((void**)lAddress) = mFreeList;
    mFreeList = (void**)lAddress;
    mUsedMemory -= mObjectSize;
    mNumAllocations--;
}