#include "stdafx.h"
#include "Shader.h"
#include "Game.h"
#include "Core/Util/Util.h"

void
cEgShaderConstantBase::Destroy()
{
    SAFE_RELEASE(mpBuffer);
}

void
cEgShaderConstantBase::ActivateInVS(
    sUInt32 luIndexSlot ) const
{
    XASSERT(mpBuffer);
    XASSERT(!mbDirty);
    Game::GetInstance().mRenderManager.ctx->VSSetConstantBuffers(luIndexSlot, 1, &mpBuffer);
}

void
cEgShaderConstantBase::ActivateInPS(
    sUInt32 luIndexSlot ) const
{
    XASSERT(mpBuffer);
    XASSERT(!mbDirty);
    Game::GetInstance().mRenderManager.ctx->PSSetConstantBuffers(luIndexSlot, 1, &mpBuffer);
}

sBool 
PixelShader::Compile(
    cTkString128Arg lacFileName,
    cTkString128Arg lacEntryPoint )
{
    gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Info, cTkDebugConsole::eDebugConsoleMode_Verbose, "[Shader] Compiling pixel shader %s", lacFileName);

    ID3DBlob* lpVSBlob = NULL;
    sBool lbSuccess = Game::GetInstance().mRenderManager.CompileShaderFromFile(lacFileName, lacEntryPoint, "ps_4_0", &lpVSBlob);
    
    if (!lbSuccess)
    {
        gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Error, cTkDebugConsole::eDebugConsoleMode_Normal, "[Shader] Error compiling pixel shader %s", lacFileName);
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
        gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Error, cTkDebugConsole::eDebugConsoleMode_Normal, "[Shader] Error creating pixel shader %s", lacFileName);

        lpVSBlob->Release();
        return FALSE;
    }

    SET_RENDER_DEBUG_NAME(mpPixelShader, "Pixel shader");


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
    cTkString128Arg   lacFileName,
    cTkString128Arg   lacEntryPoint,
    cEgVertexDecl       lLayout )
{
    gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Info, cTkDebugConsole::eDebugConsoleMode_Verbose, "[Shader] Compiling vertex shader %s", lacFileName);

    // A blob is a representation of a buffer in dx, a void pointer + size
    ID3DBlob* lpVSBlob = NULL;
    sBool lbSuccess = Game::GetInstance().mRenderManager.CompileShaderFromFile(lacFileName, lacEntryPoint, "vs_4_0", &lpVSBlob);
    if (!lbSuccess)
    {
        gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Error, cTkDebugConsole::eDebugConsoleMode_Normal, "[Shader] Error compiling vertex shader %s", lacFileName);
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
        gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Error, cTkDebugConsole::eDebugConsoleMode_Normal, "[Shader] Error creating vertex shader %s", lacFileName);
        lpVSBlob->Release();
        return FALSE;
    }

    SET_RENDER_DEBUG_NAME(mpVertexShader, "Vertex shader");

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
        gDebugConsole.Write(cTkDebugConsole::eDebugConsoleType_Error, cTkDebugConsole::eDebugConsoleMode_Normal, "[Shader] Error compiling vertex shader input layout %s", lacFileName);
        return FALSE;
    }

    SET_RENDER_DEBUG_NAME(mpVertexLayout, "Vertex layout");


    return TRUE;
}

void
VertexShader::Activate()
{
    XASSERT(mpVertexShader);
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
