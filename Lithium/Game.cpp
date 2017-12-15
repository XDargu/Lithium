#include "stdafx.h"

#include "Game.h"
#include "Core/Graphics/Shaders/Shader.h"
#include <vector>
#include "Core\Memory\FreeListAllocator.h"
#include "Core\Memory\MemoryManager.h"
#include "Core\Graphics\Mesh.h"

static Game gGame;

PixelShader     lPixelShader;
VertexShader    lVertexShader;

struct ConstantBuffer
{
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;
};

cEgMesh mTestMesh;
ID3D11Buffer*           g_pConstantBuffer = NULL;

sBool
createCube(cEgMesh& lMesh, sFloat32 size) {

    TkVector< EgVertexPosColor > vtxs;
    vtxs.resize(8);
    EgVertexPosColor *v = &vtxs[0];

    // Top
    v->Pos = XMFLOAT3(-1.0f * size, 1.0f * size, -1.0f * size); v->Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f); ++v;
    v->Pos = XMFLOAT3(1.0f * size, 1.0f * size, -1.0f * size); v->Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f); ++v;
    v->Pos = XMFLOAT3(1.0f * size, 1.0f * size, 1.0f * size); v->Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f); ++v;
    v->Pos = XMFLOAT3(-1.0f * size, 1.0f * size, 1.0f * size); v->Color = XMFLOAT4(0.f, 1.f, 0.f, 1.f); ++v;

    // Bottom
    v->Pos = XMFLOAT3(-1.0f * size, -1.0f * size, -1.0f * size); v->Color = XMFLOAT4(0.f, 0.f, 1.f, 1.f); ++v;
    v->Pos = XMFLOAT3(1.0f * size, -1.0f * size, -1.0f * size); v->Color = XMFLOAT4(0.f, 0.f, 1.f, 1.f); ++v;
    v->Pos = XMFLOAT3(1.0f * size, -1.0f * size, 1.0f * size); v->Color = XMFLOAT4(0.f, 0.f, 1.f, 1.f); ++v;
    v->Pos = XMFLOAT3(-1.0f * size, -1.0f * size, 1.0f * size); v->Color = XMFLOAT4(0.f, 0.f, 1.f, 1.f); ++v;

    // Indices
    cEgMesh::TIndex indices[] =
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

    return lMesh.Create((unsigned)vtxs.size(), &vtxs[0], ARRAYSIZE(indices), indices, cEgMesh::TRIANGLE_LIST);
}

Game& Game::GetInstance()
{
    return gGame;
}

sBool
Game::Construct()
{
    gDebugConsole.SetMinMessageLevel(cTkDebugConsole::eDebugConsoleMode::eDebugConsoleMode_Verbose);

    gMemoryManager.Construct();

    sBool lbSuccess = mRenderManager.Construct();

    ID3DBlob* lpVSBlob = NULL;

    lbSuccess &= lPixelShader.Compile("Tutorial07.fx", "PS");
    lbSuccess &= lVertexShader.Compile("Tutorial07.fx", "VS", gVertexLayoutPositionColor);
    
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    HRESULT hr = mRenderManager.device->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
    
    createCube(mTestMesh, 1);

    return lbSuccess;
}

void
Game::Update()
{
}

void
Game::Render()
{
    // Clear the back buffer 
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    mRenderManager.ctx->ClearRenderTargetView(mRenderManager.render_target_view, ClearColor);
    mRenderManager.ctx->ClearDepthStencilView(mRenderManager.depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Initialize the view matrix
    XMVECTOR Eye = XMVectorSet(0.0f, 2.0f, -5.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX g_View = XMMatrixLookAtRH(Eye, At, Up);

    // Initialize the world matrix
    XMMATRIX g_World1 = XMMatrixIdentity();

    static sFloat32 t = 0.0f;
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0)
        dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;

    g_World1 = XMMatrixRotationY(t);

    // Initialize the projection matrix
    XMMATRIX g_Projection = XMMatrixPerspectiveFovRH(XM_PIDIV2, mRenderManager.xres / (FLOAT)mRenderManager.yres, 0.01f, 100.0f);

    ConstantBuffer cb1;
    cb1.mWorld = g_World1;
    cb1.mView = g_View;
    cb1.mProjection = g_Projection;
    mRenderManager.ctx->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);

    //
    // Render the first cube
    //
    lVertexShader.Activate();
    mRenderManager.ctx->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    lPixelShader.Activate();
    mTestMesh.Activate();
    mTestMesh.Render();


    // Present the information rendered to the back buffer to the front buffer (the screen)
    mRenderManager.swap_chain->Present(0, 0);
}

sBool
Game::ShouldFinishGame()
{
    return FALSE;
}

void
Game::Release()
{
    mRenderManager.Release();
}