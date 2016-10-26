#ifndef _TYPES_H_
#define _TYPES_H_

#include "stdafx.h"

typedef char sChar32;

#define	TRUE			   1
#define	FALSE			   0

typedef int				   sBool;

typedef char               sChar;

typedef signed char        sInt8;
typedef short              sInt16;
typedef int                sInt32;
typedef long long          sInt64;
typedef unsigned char      sUInt8;
typedef unsigned short     sUInt16;
typedef unsigned int       sUInt32;
typedef unsigned long long sUInt64;

typedef float              sFloat32;
typedef double             sFloat64;

#ifdef _WIN64
typedef unsigned __int64  sUPtr;
#else
typedef unsigned int sUPtr;
#endif

#endif