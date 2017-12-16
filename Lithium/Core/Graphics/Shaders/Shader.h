#ifndef _SHADER_H_
#define _SHADER_H_

#include "VertexLayout.h"
#include "Game.h"

struct ID3D11PixelShader;
struct ID3D11VertexShader;
struct ID3D11InputLayout;

class cEgShaderConstantBase 
{
public:
    
    cEgShaderConstantBase() : mpBuffer(nullptr), mbDirty(false) {}
    
    void 
    Destroy();

    void 
    ActivateInVS(
        sUInt32 luIndexSlot ) const;
    
    void
    ActivateInPS(
        sUInt32 luIndexSlot ) const;

protected:
    
    ID3D11Buffer * mpBuffer;
    sBool          mbDirty;
};

template< class TParams >
class cEgShaderConstant : public cEgShaderConstantBase
{
    
public:
    sBool
    Create()
    {
        POW2_ASSERT(mpBuffer == nullptr);

        // Create the constant buffer
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(TParams);
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;

        HRESULT hr = Game::GetInstance().mRenderManager.device->CreateBuffer(&bd, NULL, &mpBuffer);
        if (FAILED(hr))
            return FALSE;

        return TRUE;
    }

    TParams* 
    Get()
    {
        mbDirty = TRUE;
        return &mParams;
    }

    void 
    UploadToGPU()
    {
        POW2_ASSERT(mpBuffer);

        mbDirty = FALSE;
        const TParams *obj = &mParams;
        Game::GetInstance().mRenderManager.ctx->UpdateSubresource(mpBuffer, 0, NULL, obj, 0, 0);
    }

private:
    
    TParams mParams;
};

class PixelShader
{
public:
    PixelShader() : mpPixelShader(nullptr)
    {}

    sBool
    Compile(
        cTkString128Arg lacFileName,
        cTkString128Arg lacEntryPoint );

    void
    Activate();

    void
    Release();

private:
    ID3D11PixelShader*   mpPixelShader;
};

class VertexShader
{
public:
    VertexShader() 
        : mpVertexShader(nullptr)
        , mpVertexLayout(nullptr)
    {}

    sBool
    Compile(
        cTkString128Arg   lacFileName,
        cTkString128Arg   lacEntryPoint,
        cEgVertexDecl       lLayout);

    void
    Activate();

    void
    Release();

private:
    ID3D11VertexShader*   mpVertexShader;
    ID3D11InputLayout*    mpVertexLayout;
};

#endif
