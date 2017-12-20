#ifndef _DEBUG_GLOBALS_H
#define _DEBUG_GLOBALS_H

#include "Core\Types\Types.h"
#include "Core\Graphics\Mesh.h"

class cEgMesh;

class cGcDebugGlobals
{
public:
    
    cGcDebugGlobals();

    cEgMesh  mGridMesh;
    sBool    mbDebugWireframe;
};

extern cGcDebugGlobals gDebugGlobals;

#endif