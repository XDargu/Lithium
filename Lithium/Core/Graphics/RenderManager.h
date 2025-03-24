#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

// Includes
#pragma warning( push )
#pragma warning( disable: 4005 )
#include <d3d11.h>
#include <d3dcompiler.h>
#include <D3Dcommon.h>
#include <d3d11.h>
#include <d3d9.h>         // D3DPERF_*
#pragma warning( pop )

#pragma comment( lib, "dxguid.lib")

// Compilation defines
#define SET_RENDER_DEBUG_NAME(obj,name) (obj)->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT) strlen( name ), name );

// Forward declarations

// Classes

class cEgRenderManager 
{
public:
	HWND					hWnd;
	ID3D11Device*           device;
	ID3D11DeviceContext*    ctx;
	IDXGISwapChain*         swap_chain;
	ID3D11RenderTargetView* render_target_view;
	ID3D11Texture2D*        depth_stencil;
	ID3D11DepthStencilView* depth_stencil_view;
	sInt32                  xres, yres;

	// Methods
	cEgRenderManager();
	sBool Construct();

    sBool ConstructDevice();

    void Release();

    sBool
    cEgRenderManager::CompileShaderFromFile(
        cTkString128Arg   lacFileName,
        cTkString128Arg   lacEntryPoint,
        cTkString128Arg   lacShaderModel,
        ID3DBlob**        lppBlobOut);

	void ActivateBackbuffer();

};

#endif