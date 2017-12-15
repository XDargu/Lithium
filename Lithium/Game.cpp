#include "stdafx.h"

#include "Game.h"
#include "Core/Graphics/Shaders/Shader.h"
#include <vector>
#include "Core\Memory\FreeListAllocator.h"
#include "Core\Memory\MemoryManager.h"

#pragma warning( push )
#pragma warning( disable: 4838 )
#include <xnamath.h>
#pragma warning( pop )

static Game gGame;

PixelShader     lPixelShader;
VertexShader    lVertexShader;

ID3D11Buffer*           g_pVertexBuffer = NULL;

struct SimpleVertex
{
    XMFLOAT3 Pos; 
};

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
    lbSuccess &= lVertexShader.Compile("Tutorial07.fx", "VS", gVertexLayoutPosition);

    

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        XMFLOAT3(0.0f, 0.5f, 0.5f),
        XMFLOAT3(0.5f, -0.5f, 0.5f),
        XMFLOAT3(-0.5f, -0.5f, 0.5f),
    };

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    HRESULT hr = mRenderManager.device->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
        return hr;

    // Set vertex buffer
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    mRenderManager.ctx->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // Set primitive topology
    mRenderManager.ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);    

    TkVector<sInt32, cEgMemoryManager::Renderer> lTest;

    TkMap<sInt32, String128> lMyMap;

    lMyMap[2] = "Hey";
    lMyMap[56] = "Hallo";
    lMyMap[987] = "Howdy";

    for (sInt32 i = 0; i < 200; i++)
    {
        lTest.push_back(i);
    }

    lTest.push_back(2);

    lTest.clear();
    lTest.resize(0);

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

    // Render a triangle
    lPixelShader.Activate();
    lVertexShader.Activate();
    mRenderManager.ctx->Draw(3, 0);

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