#include "stdafx.h"
#include "ProxyAllocator.h"

ProxyAllocator::ProxyAllocator(
    Allocator& lAllocator)
    : Allocator(lAllocator.GetSize(), lAllocator.GetStart()), mAllocator(lAllocator)
{
}

ProxyAllocator::~ProxyAllocator()
{
}

void*
ProxyAllocator::Allocate(
    size_t lSize, 
    sUInt8 lAlignment )
{
    assert(lSize != 0);
    mNumAllocations++;

    size_t lMem = mAllocator.GetUsedMemory();
    void* p = mAllocator.Allocate(lSize, lAlignment);
    mUsedMemory += mAllocator.GetUsedMemory() - lMem;

    return p;
}

void 
ProxyAllocator::Deallocate(
    void* lAddress)
{
    mNumAllocations--;
    size_t mem = mAllocator.GetUsedMemory();
    mAllocator.Deallocate(lAddress);
    mUsedMemory -= mem - mAllocator.GetUsedMemory();
}