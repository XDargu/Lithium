#include "stdafx.h"
#include "VertexLayout.h"

#define DEF_VERTEX_LAYOUT(x) VerexLayout x(x##Layout, ARRAYSIZE(x##Layout)) 

sUInt32
GetBytesOfFormat(
    DXGI_FORMAT leFormat ) 
{
    switch (leFormat)
    {
    case DXGI_FORMAT_R32_SINT: return 4;
    case DXGI_FORMAT_R32G32_FLOAT: return 8;
    case DXGI_FORMAT_R32G32B32_FLOAT: return 12;
    case DXGI_FORMAT_R32G32B32A32_FLOAT: return 16;
    case DXGI_FORMAT_R8G8B8A8_UINT: return 4;
    case DXGI_FORMAT_R8G8B8A8_UNORM: return 4;
    case DXGI_FORMAT_R32_FLOAT: return 4;
    }
    return 0;
}

VerexLayout::VerexLayout(
    D3D11_INPUT_ELEMENT_DESC*   lpElems, 
    UINT                        luNumElems)
    : mpElems(lpElems)
    , muNumElems(luNumElems)
    , muBytesPerVertex(0)
{
    // Get the total amount of bytes per vertex
    for (sUInt32 liIndex = 0; liIndex < muNumElems; ++liIndex) {
        muBytesPerVertex += GetBytesOfFormat(mpElems[liIndex].Format);
    }
}

D3D11_INPUT_ELEMENT_DESC gVertexLayoutPositionLayout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

DEF_VERTEX_LAYOUT(gVertexLayoutPosition);

D3D11_INPUT_ELEMENT_DESC gVertexLayoutPositionUVLayout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

DEF_VERTEX_LAYOUT(gVertexLayoutPositionUV);
