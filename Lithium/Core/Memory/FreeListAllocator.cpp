#include "stdafx.h"
#include "FreeListAllocator.h"
#include "Core\Util\PointerMath.h"

FreeListAllocator::FreeListAllocator(size_t lSize, void* lStart)
    : Allocator(lSize, lStart), mFreeBlocks((FreeBlock*)lStart)
{
    assert(lSize > sizeof(FreeBlock));
    mFreeBlocks->lSize = lSize;
    mFreeBlocks->lNext = nullptr;
}
FreeListAllocator::~FreeListAllocator()
{
    mFreeBlocks = nullptr;
}
void* 
FreeListAllocator::Allocate(
    size_t lSize, 
    sUInt8 lAlignment )
{
    assert(lSize != 0 && lAlignment != 0);
    FreeBlock* lPrevFreeBlock = nullptr;
    FreeBlock* lFreeBlock = mFreeBlocks;
    while (lFreeBlock != nullptr)
    {
        //Calculate adjustment needed to keep object correctly aligned
        sUInt8 adjustment = pointer_math::AlignForwardAdjustmentWithHeader(lFreeBlock, lAlignment, sizeof(AllocationHeader));
        size_t lTotalSize = lSize + adjustment;
        //If allocation doesn't fit in this FreeBlock, try the next
        if (lFreeBlock->lSize < lTotalSize)
        {
            lPrevFreeBlock = lFreeBlock;
            lFreeBlock = lFreeBlock->lNext;
            continue;
        }
        static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");
        //If allocations in the remaining memory will be impossible
        if (lFreeBlock->lSize - lTotalSize <= sizeof(AllocationHeader))
        {
            //Increase allocation size instead of creating a new FreeBlock
            lTotalSize = lFreeBlock->lSize;
            if (lPrevFreeBlock != nullptr)
                lPrevFreeBlock->lNext = lFreeBlock->lNext;
            else
                mFreeBlocks = lFreeBlock->lNext;
        }
        else
        {
            //Else create a new FreeBlock containing remaining memory
            FreeBlock* next_block = (FreeBlock*)(pointer_math::Add(lFreeBlock, lTotalSize));
            next_block->lSize = lFreeBlock->lSize - lTotalSize;
            next_block->lNext = lFreeBlock->lNext;
            if (lPrevFreeBlock != nullptr)
                lPrevFreeBlock->lNext = next_block;
            else
                mFreeBlocks = next_block;
        }
        sUPtr aligned_address = (sUPtr)lFreeBlock + adjustment;
        AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
        header->lSize = lTotalSize;
        header->lAdjustment = adjustment;
        mUsedMemory += lTotalSize;
        mNumAllocations++;
        assert(pointer_math::AlignForwardAdjustment((void*)aligned_address, lAlignment) == 0);
        return (void*)aligned_address;
    }
    //ASSERT(false && "Couldn't find free block large enough!");
    return nullptr;
}
void 
FreeListAllocator::Deallocate(
    void* lAddress )
{
    assert(lAddress != nullptr);
    AllocationHeader* lHeader = (AllocationHeader*)pointer_math::Subtract(lAddress, sizeof(AllocationHeader));
    sUPtr lBlockStart = reinterpret_cast<sUPtr>(lAddress)-lHeader->lAdjustment;
    size_t lBlockSize = lHeader->lSize;
    sUPtr lBlockEnd = lBlockStart + lBlockSize;
    FreeBlock* lPrevFreeBlock = nullptr;
    FreeBlock* lFreeBlock = mFreeBlocks;
    while (lFreeBlock != nullptr)
    {
        if ((sUPtr)mFreeBlocks >= lBlockEnd)
            break;
        lPrevFreeBlock = lFreeBlock;
        lFreeBlock = lFreeBlock->lNext;
    }
    if (lPrevFreeBlock == nullptr)
    {
        lPrevFreeBlock = (FreeBlock*)lBlockStart;
        lPrevFreeBlock->lSize = lBlockSize;
        lPrevFreeBlock->lNext = mFreeBlocks;
        mFreeBlocks = lPrevFreeBlock;
    }
    else if ((sUPtr)lPrevFreeBlock + lPrevFreeBlock->lSize == lBlockStart)
    {
        lPrevFreeBlock->lSize += lBlockSize;
    }
    else
    {
        FreeBlock* temp = (FreeBlock*)lBlockStart;
        temp->lSize = lBlockSize;
        temp->lNext = lPrevFreeBlock->lNext;
        lPrevFreeBlock->lNext = temp;
        lPrevFreeBlock = temp;
    }
    if (lFreeBlock != nullptr && (sUPtr)lFreeBlock == lBlockEnd)
    {
        lPrevFreeBlock->lSize += lFreeBlock->lSize;
        lPrevFreeBlock->lNext = lFreeBlock->lNext;
    }
    mNumAllocations--;
    mUsedMemory -= lBlockSize;
}