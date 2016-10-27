#include "stdafx.h"
#include "Game.h"
#pragma warning( push )
#pragma warning( disable: 4838 )
#include <xnamath.h>
#pragma warning( pop )

static Game gGame;

ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
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
    sBool lbSuccess = mRenderManager.Construct();

    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    mRenderManager.CompileShaderFromFile("Tutorial07.fx", "VS", "vs_4_0", &pVSBlob);


    // Create the vertex shader
    HRESULT hr = mRenderManager.device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return hr;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    hr = mRenderManager.device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);
    pVSBlob->Release();
    if (FAILED(hr))
        return hr;

    // Set the input layout
    mRenderManager.ctx->IASetInputLayout(g_pVertexLayout);

    // Compile the pixel shader
    ID3DBlob* pPSBlob = NULL;
    mRenderManager.CompileShaderFromFile("Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);


    // Create the pixel shader
    hr = mRenderManager.device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return hr;

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
    hr = mRenderManager.device->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
        return hr;

    // Set vertex buffer
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    mRenderManager.ctx->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // Set primitive topology
    mRenderManager.ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
    mRenderManager.ctx->VSSetShader(g_pVertexShader, NULL, 0);
    mRenderManager.ctx->PSSetShader(g_pPixelShader, NULL, 0);
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