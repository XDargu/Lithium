#ifndef _SHADER_H_
#define _SHADER_H_

#include "VertexLayout.h"

struct ID3D11PixelShader;
struct ID3D11VertexShader;
struct ID3D11InputLayout;

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