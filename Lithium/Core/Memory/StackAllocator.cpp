#include "stdafx.h"
#include "StackAllocator.h"
#include "Core\Util\PointerMath.h"

StackAllocator::StackAllocator(size_t lSize, void* lStart)
    : Allocator(lSize, lStart), mCurrentPos(lStart)
{
    assert(lSize > 0);
#if _DEBUG
    mPrevPos = nullptr;
#endif
}

StackAllocator::~StackAllocator()
{
#if _DEBUG
    mPrevPos = nullptr;
#endif
    mCurrentPos = nullptr;
}

void* StackAllocator::Allocate(
    size_t lSize,
    sUInt8 lAlignment)
{
    assert(lSize != 0);

    sUInt8 lAdjustment = pointer_math::AlignForwardAdjustmentWithHeader(mCurrentPos, lAlignment, sizeof(AllocationHeader));

    if (mUsedMemory + lAdjustment + lSize > lSize)
        return nullptr;

    void* lAlignedAddress = pointer_math::Add(mCurrentPos, lAdjustment);

    //Add Allocation Header
    AllocationHeader* header = (AllocationHeader*)(pointer_math::Subtract(lAlignedAddress, sizeof(AllocationHeader)));
    header->lAdjustment = lAdjustment;

#if _DEBUG
    header->mPrevAddress = mPrevPos;
    mPrevPos = lAlignedAddress;
#endif
    mCurrentPos = pointer_math::Add(lAlignedAddress, lSize);
    mUsedMemory += lSize + lAdjustment;
    mNumAllocations++;
    return lAlignedAddress;
}

void StackAllocator::Deallocate(
    void* lAddress)
{
    assert(lAddress == mPrevPos);

    //Access the AllocationHeader in the bytes before p
    AllocationHeader* header = (AllocationHeader*)(pointer_math::Subtract(lAddress, sizeof(AllocationHeader)));
    mUsedMemory -= (sUPtr)mCurrentPos - (sUPtr)lAddress + header->lAdjustment;
    mCurrentPos = pointer_math::Subtract(lAddress, header->lAdjustment);

#if _DEBUG
    mPrevPos = header->mPrevAddress;
#endif

    mNumAllocations--;
}