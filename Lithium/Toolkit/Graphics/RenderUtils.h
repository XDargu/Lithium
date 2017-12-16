#ifndef _TKRENDER_UTILS_H_
#define _TKRENDER_UTILS_H_

#include "Core/Graphics/Mesh.h"

namespace render_utils
{
    sBool
    CreateGrid(
        cEgMesh& lMesh,
        sInt32 liNumSamples);

    sBool
    CreateCube(
        cEgMesh& lMesh,
        sFloat32 lfSize);

}

namespace render_constants
{
    void
    Construct();

    void
    Release();

    void
    SetViewProjection(
        XMMATRIX lView,
        XMMATRIX lProjection);

    void
    SetWorldMatrix(
        XMMATRIX lWorld );

    void 
    ActivateConstantBuffer(
        sInt32 liSlot );
}

#endif