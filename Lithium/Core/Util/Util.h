#ifndef _UTIL_H_
#define _UTIL_H_

#define SAFE_RELEASE(x) if( x ) x->Release(), x = nullptr

#endif