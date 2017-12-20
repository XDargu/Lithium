#include "stdafx.h"

#include "Game.h"
#include "Core/Graphics/Shaders/Shader.h"
#include <vector>
#include "Core\Memory\FreeListAllocator.h"
#include "Core\Memory\MemoryManager.h"
#include "Core\Globals\DebugGlobals.h"

static Game gGame;

PixelShader     lPixelShader;
VertexShader    lVertexShader;

cEgMesh mTestMesh;

Game& Game::GetInstance()
{
    return gGame;
}

sBool
Game::Construct()
{
    sBool lbSuccess = mRenderManager.Construct();

    ID3DBlob* lpVSBlob = NULL;

    lbSuccess &= lPixelShader.Compile("Tutorial07.fx", "PS");
    lbSuccess &= lVertexShader.Compile("Tutorial07.fx", "VS", gVertexLayoutPositionColor);
    
    render_constants::Construct();
    
    render_utils::CreateCube(mTestMesh, 1);
    render_utils::CreateGrid(gDebugGlobals.mGridMesh, 10);

    return lbSuccess;
}

void
Game::Update()
{
}

void
Game::Render()
{
    // Time
    static sFloat32 t = 0.0f;
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0)
        dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;

    // Clear the back buffer 
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    mRenderManager.ctx->ClearRenderTargetView(mRenderManager.render_target_view, ClearColor);
    mRenderManager.ctx->ClearDepthStencilView(mRenderManager.depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Initialize the view matrix
    Vector3 lEye = Vector3(5.0f * sin(t*0.5f), 2.0f + sin(t), 5.0f * cos(t*0.5f));
    Vector3 lAt = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 lUp = Vector3(0.0f, 1.0f, 0.0f);

    Matrix lView = Matrix::CreateLookAt(lEye, lAt, lUp);

    // Initialize the projection matrix
    Matrix lProjection = Matrix::CreatePerspectiveFieldOfView(XM_PIDIV2, mRenderManager.xres / (FLOAT)mRenderManager.yres, 0.01f, 100.0f);

    render_constants::ActivateConstantBuffer(0);

    render_constants::SetViewProjection(lView, lProjection);

    // Initialize the world matrix
    Matrix lWorld1 = Matrix::Identity;

    lWorld1 = XMMatrixRotationY(-t);
    
    render_constants::SetWorldMatrix(lWorld1);

    //
    // Render the first cube
    //
    lVertexShader.Activate();
    lPixelShader.Activate();

    cEgMesh::Activate(mTestMesh);
    cEgMesh::Render(mTestMesh);

    // 
    lWorld1 = Matrix::Identity;
    render_constants::SetWorldMatrix(lWorld1);

    //
    // Render the grid
    //
    cEgMesh::Activate(gDebugGlobals.mGridMesh);
    cEgMesh::Render(gDebugGlobals.mGridMesh);

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
    render_constants::Release();
}