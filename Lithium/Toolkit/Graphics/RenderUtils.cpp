#include "stdafx.h"

#include "RenderUtils.h"

#include "Core\Graphics\Shaders\Constants\ShaderConstants.h"
#include "Core\Graphics\Shaders\Shader.h"

sBool 
render_utils::CreateGrid(
    cEgMesh& lMesh, 
    sInt32 liNumSamples)
{

    TkVector< EgVertexPosColor > laVertices;

    XMFLOAT4 c2 = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    XMFLOAT4 c1 = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);

    sFloat32 fsamples = (sFloat32)liNumSamples;
    EgVertexPosColor v;

    // The lines from -x to +x
    for (int z = -liNumSamples; z <= liNumSamples; ++z)
    {
        v.Color = (z % 5) ? c2 : c1;
        v.Pos = XMFLOAT3(-fsamples, 0.f, (float)z);
        laVertices.push_back(v);
        v.Pos = XMFLOAT3(fsamples, 0.f, (float)z);
        laVertices.push_back(v);
    }

    // The lines from -z to +z
    for (int x = -liNumSamples; x <= liNumSamples; ++x)
    {
        v.Color = (x % 5) ? c2 : c1;
        v.Pos = XMFLOAT3((float)x, 0.f, -fsamples);
        laVertices.push_back(v);
        v.Pos = XMFLOAT3((float)x, 0.f, fsamples);
        laVertices.push_back(v);
    }

    return lMesh.Create((unsigned)laVertices.size(), &laVertices[0], 0, nullptr, cEgMesh::LINE_LIST);
}

sBool
render_utils::CreateCube(
    cEgMesh& lMesh, 
    sFloat32 lfSize) 
{

    TkVector< EgVertexPosColor > laVertices;
    laVertices.resize(8);
    EgVertexPosColor *v = &laVertices[0];

    // Top
    v->Pos = XMFLOAT3(-1.0f * lfSize, 1.0f * lfSize, -1.0f * lfSize); v->Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f); ++v;
    v->Pos = XMFLOAT3(1.0f * lfSize, 1.0f * lfSize, -1.0f * lfSize); v->Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f); ++v;
    v->Pos = XMFLOAT3(1.0f * lfSize, 1.0f * lfSize, 1.0f * lfSize); v->Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f); ++v;
    v->Pos = XMFLOAT3(-1.0f * lfSize, 1.0f * lfSize, 1.0f * lfSize); v->Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f); ++v;

    // Bottom
    v->Pos = XMFLOAT3(-1.0f * lfSize, -1.0f * lfSize, -1.0f * lfSize); v->Color = XMFLOAT4(0.f, 0.f, 1.f, 1.f); ++v;
    v->Pos = XMFLOAT3(1.0f * lfSize, -1.0f * lfSize, -1.0f * lfSize); v->Color = XMFLOAT4(0.f, 0.f, 1.f, 1.f); ++v;
    v->Pos = XMFLOAT3(1.0f * lfSize, -1.0f * lfSize, 1.0f * lfSize); v->Color = XMFLOAT4(0.f, 0.f, 1.f, 1.f); ++v;
    v->Pos = XMFLOAT3(-1.0f * lfSize, -1.0f * lfSize, 1.0f * lfSize); v->Color = XMFLOAT4(0.f, 0.f, 1.f, 1.f); ++v;

    // Indices
    cEgMesh::TIndex laiIndices[] =
    {
        3, 0, 1,
        2, 3, 1,

        0, 4, 5,
        1, 0, 5,

        3, 7, 4,
        0, 3, 4,

        1, 5, 6,
        2, 1, 6,

        2, 6, 7,
        3, 2, 7,

        6, 5, 4,
        7, 6, 4,
    };

    return lMesh.Create((unsigned)laVertices.size(), &laVertices[0], ARRAYSIZE(laiIndices), laiIndices, cEgMesh::TRIANGLE_LIST);
}

namespace render_constants
{
    cEgShaderConstant<cEgConstantBuffer> gConstantBuffer;

    void
    Construct()
    {
        gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Info, cTkDebugConsole::eDebugConsoleMode_Normal, "[Render Constants] Creating Render Constants");
        gConstantBuffer.Create();
    }

    void
    Release()
    {
        gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Info, cTkDebugConsole::eDebugConsoleMode_Normal, "[Render Constants] Raleasing Render Constants");
        gConstantBuffer.Destroy();
    }

    void
    SetViewProjection(
        XMMATRIX lView, 
        XMMATRIX lProjection )
    {
        gConstantBuffer.Get()->View = lView;
        gConstantBuffer.Get()->Projection = lProjection;
        gConstantBuffer.UploadToGPU();
    }

    void 
    SetWorldMatrix(
        XMMATRIX lWorld )
    {
        gConstantBuffer.Get()->World = lWorld;
        gConstantBuffer.UploadToGPU();
    }

    void 
    ActivateConstantBuffer(
        sInt32 liSlot )
    {
        gConstantBuffer.ActivateInVS(liSlot);
        gConstantBuffer.ActivateInPS(liSlot);
    }

}