#ifndef _FREELIST_ALLOCATOR_H
#define _FREELIST_ALLOCATOR_H


#include "Allocator.h"

class FreeListAllocator : public Allocator
{
public:
    FreeListAllocator(
        size_t lSize, 
        void* lStart);

    ~FreeListAllocator();

    void* 
    Allocate(
        size_t lSize, 
        sUInt8 lAlignment ) override;

    void
    Deallocate(
        void* lAddress ) override;

private:

    struct AllocationHeader
    {
        size_t lSize;
        sUInt8 lAdjustment;
    };

    struct FreeBlock
    {
        size_t lSize;
        FreeBlock* lNext;
    };

    FreeListAllocator(const FreeListAllocator&); //Prevent copies because it might cause errors
    FreeListAllocator& operator=(const FreeListAllocator&);
    FreeBlock* mFreeBlocks;
};

#endif