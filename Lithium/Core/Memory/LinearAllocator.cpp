#include "stdafx.h"

#include "LinearAllocator.h"
#include "Core\Util\PointerMath.h"

void * 
LinearAllocator::Allocate(
    size_t lSize, 
    sUInt8 mAlignement)
{
    assert(lSize > 0);

    sUInt8 lPadding = pointer_math::AlignForwardPadding(mCurrentPos, mAlignement);

    if (mUsedMemory + lPadding + lSize > mSize)
    {
        return nullptr;
    }

    sUPtr lAlignedAddress = (sUPtr)mCurrentPos + lPadding;

    mCurrentPos = (void*)(lAlignedAddress + lSize);

    mUsedMemory += lSize + lPadding;
    mNumAllocations++;

    return (void*)lAlignedAddress;
}
