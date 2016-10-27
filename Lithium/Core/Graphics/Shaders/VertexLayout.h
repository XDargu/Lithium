#ifndef INC_VERTEX_LAYOUT_H_
#define INC_VERTEX_LAYOUT_H_

#include "Core/Graphics/RenderManager.h"

class VerexLayout {
public:
    
    VerexLayout(
        D3D11_INPUT_ELEMENT_DESC*   lpElems, 
        UINT                        luNumElems );

    D3D11_INPUT_ELEMENT_DESC*   mpElems;
    UINT                        muNumElems;
    sUInt32                     muBytesPerVertex;
};

extern VerexLayout gVertexLayoutPosition;
extern VerexLayout gVertexLayoutPositionUV;

#endif