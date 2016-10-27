#include "stdafx.h"
#include "Shader.h"
#include "Game.h"
#include "Core/Util/Util.h"

sBool 
PixelShader::Compile(
    String128 lacFileName,
    String128 lacEntryPoint )
{
    gDebugConsole.Write(DebugConsole::eDebugConsoleType_Info, DebugConsole::eID3D11InputLayout_Verbose, "[Shader] Compiling pixel shader %s", lacFileName);

    ID3DBlob* lpVSBlob = NULL;
    sBool lbSuccess = Game::GetInstance().mRenderManager.CompileShaderFromFile(lacFileName, lacEntryPoint, "ps_4_0", &lpVSBlob);
    
    if (!lbSuccess)
    {
        gDebugConsole.Write(DebugConsole::eDebugConsoleType_Error, DebugConsole::eID3D11InputLayout_Verbose, "[Shader] Error compiling pixel shader %s", lacFileName);
        return FALSE;
    }

    // Create the vertex shader
    HRESULT hr;
    hr = Game::GetInstance().mRenderManager.device->CreatePixelShader(
        lpVSBlob->GetBufferPointer()
        , lpVSBlob->GetBufferSize()
        , NULL
        , &mpPixelShader);
    if (FAILED(hr))
    {
        gDebugConsole.Write(DebugConsole::eDebugConsoleType_Error, DebugConsole::eID3D11InputLayout_Verbose, "[Shader] Error creating pixel shader %s", lacFileName);

        lpVSBlob->Release();
        return FALSE;
    }

    lpVSBlob->Release();
    
    return TRUE;
}

void 
PixelShader::Activate()
{
    Game::GetInstance().mRenderManager.ctx->PSSetShader(mpPixelShader, NULL, 0);
}

void 
PixelShader::Release()
{
    SAFE_RELEASE(mpPixelShader);
}

sBool
VertexShader::Compile(
    String128   lacFileName,
    String128   lacEntryPoint,
    VerexLayout lLayout )
{
    gDebugConsole.Write(DebugConsole::eDebugConsoleType_Info, DebugConsole::eID3D11InputLayout_Verbose, "[Shader] Compiling vertex shader %s", lacFileName);

    // A blob is a representation of a buffer in dx, a void pointer + size
    ID3DBlob* lpVSBlob = NULL;
    sBool lbSuccess = Game::GetInstance().mRenderManager.CompileShaderFromFile(lacFileName, lacEntryPoint, "vs_4_0", &lpVSBlob);
    if (!lbSuccess)
    {
        gDebugConsole.Write(DebugConsole::eDebugConsoleType_Error, DebugConsole::eID3D11InputLayout_Verbose, "[Shader] Error compiling vertex shader %s", lacFileName);
        return FALSE;
    }
    // Create the vertex shader
    HRESULT hr;
    hr = Game::GetInstance().mRenderManager.device->CreateVertexShader(
        lpVSBlob->GetBufferPointer(),
        lpVSBlob->GetBufferSize(),
        NULL,
        &mpVertexShader);

    if (FAILED(hr))
    {
        gDebugConsole.Write(DebugConsole::eDebugConsoleType_Error, DebugConsole::eID3D11InputLayout_Verbose, "[Shader] Error creating vertex shader %s", lacFileName);
        lpVSBlob->Release();
        return FALSE;
    }

    // Create the input layout
    hr = Game::GetInstance().mRenderManager.device->CreateInputLayout(
        lLayout.mpElems, 
        lLayout.muNumElems, 
        lpVSBlob->GetBufferPointer(),
        lpVSBlob->GetBufferSize(), 
        &mpVertexLayout
    );

    lpVSBlob->Release();

    if (FAILED(hr))
    {
        gDebugConsole.Write(DebugConsole::eDebugConsoleType_Error, DebugConsole::eID3D11InputLayout_Verbose, "[Shader] Error compiling vertex shader input layout %s", lacFileName);
        return FALSE;
    }

    return TRUE;
}

void
VertexShader::Activate()
{
    assert(mpVertexShader);
    Game::GetInstance().mRenderManager.ctx->VSSetShader(mpVertexShader, NULL, 0);

    // Set the input layout
    Game::GetInstance().mRenderManager.ctx->IASetInputLayout(mpVertexLayout);
}

void
VertexShader::Release()
{
    SAFE_RELEASE(mpVertexLayout);
    SAFE_RELEASE(mpVertexShader);
}
