#ifndef _TKTYPES_H_
#define _TKTYPES_H_

#include <vector>
#include <map>
#include "Core\Memory\MemoryManager.h"

template <class T, cEgMemoryManager::MemoryPool Pool = cEgMemoryManager::CurrentPool>
using TkVector = std::vector<T, CustomAllocator<T, Pool>>;

template <class Key, class T, cEgMemoryManager::MemoryPool Pool = cEgMemoryManager::CurrentPool>
using TkMap = std::map<Key, T, std::less<Key>, CustomAllocator<T, Pool>>;



#endif