#ifndef INC_VERTEX_LAYOUT_H_
#define INC_VERTEX_LAYOUT_H_

#include "Core/Graphics/RenderManager.h"

class cEgVertexDecl {
public:
    
    cEgVertexDecl(
        D3D11_INPUT_ELEMENT_DESC*   lpElems, 
        UINT                        luNumElems );

    D3D11_INPUT_ELEMENT_DESC*   mpElems;
    UINT                        muNumElems;
    sUInt32                     muBytesPerVertex;
};

struct EgVertexPos {
    XMFLOAT3 Pos;
};

struct EgVertexPosColor {
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

template<class TVertex>
cEgVertexDecl* GetVertexDecl();

extern cEgVertexDecl gVertexLayoutPosition;
extern cEgVertexDecl gVertexLayoutPositionUV;
extern cEgVertexDecl gVertexLayoutPositionColor;

#endif