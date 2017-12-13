#ifndef _STACK_ALLOCATOR_H
#define _STACK_ALLOCATOR_H

#include "Allocator.h"

class StackAllocator : public Allocator
{
public:

    StackAllocator(size_t lSize, void* lStart);

    ~StackAllocator();

    void* Allocate(size_t lSize, sUInt8 lAlignment) override;

    void Deallocate(void* lPointer) override;

private:

    StackAllocator(const StackAllocator&); //Prevent copies because it might cause errors
    StackAllocator& operator=(const StackAllocator&);

    struct AllocationHeader
    {
#if _DEBUG
        void* mPrevAddress;
#endif
        sUInt8 lAdjustment;
    };
#if _DEBUG

    void* mPrevPos;
#endif
    void* mCurrentPos;
};

#endif
