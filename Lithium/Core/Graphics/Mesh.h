#ifndef _MESH_H_
#define _MESH_H_

// Includes
#pragma warning( push )
#pragma warning( disable: 4005 )
#include <d3d11.h>
#include <d3dcompiler.h>
#include <D3Dcommon.h>
#include <d3dx11.h>
#include <d3d9.h>         // D3DPERF_*
#pragma warning( pop )

#include "Core\Graphics\Shaders\VertexLayout.h"

class cEgMesh
{
    ID3D11Buffer*   mVertexBuffer;
    ID3D11Buffer*   mIndexBuffer;

    sUInt32         muNumVertices;
    sUInt32         muNumIndices;

    D3D_PRIMITIVE_TOPOLOGY    mTopology;    // TriangleList, POINTS, LINES...
    const cEgVertexDecl*      mVertexDecl;   // The type of vertexs

public:

    typedef sUInt16 TIndex;

    enum ePrimitiveType {
        POINTS = 4000,
        LINE_LIST,
        TRIANGLE_LIST,
        TRIANGLE_STRIP,
        LINE_LIST_ADJ
    };

    cEgMesh();
    ~cEgMesh();

    sBool
    Create(
        sUInt32              luNumVertices,
        const void*          lpVertices,
        sUInt32              luNumIndices,
        const TIndex*        lpIndices,
        ePrimitiveType       lePrimitiveType,
        const cEgVertexDecl* lVertexDecl,
        sBool                lbMutable = FALSE
    );

    // Create using a vertex type
    template< class TVertex >
    sBool
    Create(
        sUInt32              luNumVertices,
        const TVertex*       lpVertices,
        sUInt32              luNumIndices,
        const TIndex*        lpIndices,
        ePrimitiveType       lePrimitiveType,
        sBool                lbMutable = FALSE
    ) 
    {
        return Create(
            luNumVertices,
            lpVertices,
            luNumIndices,
            lpIndices,
            lePrimitiveType,
            GetVertexDecl<TVertex>(),
            lbMutable
        );
    }

    void
    cEgMesh::UpdateFromCPU(
        const void *    lpNewCPUData,
        size_t          luNumBytesToUpdate);

    void
    cEgMesh::Activate() const;

    void
    cEgMesh::Render() const;
};

#endif