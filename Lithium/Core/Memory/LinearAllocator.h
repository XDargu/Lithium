#ifndef _LINEAR_ALLOCATOR_H
#define _LINEAR_ALLOCATOR_H

#include "Allocator.h"

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
    Allocate(
        size_t lSize, 
        sUInt8 mAlignement ) override;

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