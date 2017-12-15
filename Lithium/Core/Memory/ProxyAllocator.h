#ifndef _PROXY_ALLOCATOR_H
#define _PROXY_ALLOCATOR_H

#include "Allocator.h"

class ProxyAllocator : public Allocator
{
public:
    ProxyAllocator(Allocator& lAllocator);
    ~ProxyAllocator();

    void* 
    Allocate(
        size_t lSize, 
        sUInt8 lAlignment ) override;

    void 
    Deallocate(
        void* lAddress ) override;

private:
    ProxyAllocator(const ProxyAllocator&); //Prevent copies because it might cause errors
    ProxyAllocator& operator=(const ProxyAllocator&);

    Allocator& mAllocator;
};

#endif