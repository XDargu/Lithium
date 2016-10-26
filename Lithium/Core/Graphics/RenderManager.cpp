#include "stdafx.h"
#include <d3dx11.h>
#include "RenderManager.h"
#include "Core\Util\Util.h"

RenderManager::RenderManager()
	: device(nullptr)
	, ctx(nullptr)
	, swap_chain(nullptr)
	, render_target_view(nullptr)
	, depth_stencil(nullptr)
	, depth_stencil_view(nullptr)
	, hWnd(0)
{ }

// --------------------------------------
sBool 
RenderManager::Construct() 
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		//D3D_FEATURE_LEVEL_10_1,
		//D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL       featureLevel = D3D_FEATURE_LEVEL_11_0;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		D3D_DRIVER_TYPE g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &swap_chain, &device, &featureLevel, &ctx);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return FALSE;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return FALSE;

	hr = device->CreateRenderTargetView(pBackBuffer, NULL, &render_target_view);
	pBackBuffer->Release();
	if (FAILED(hr))
		return FALSE;

	// ------------------------------------
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = device->CreateTexture2D(&descDepth, NULL, &depth_stencil);
	if (FAILED(hr))
		return FALSE;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depth_stencil, &descDSV, &depth_stencil_view);
	if (FAILED(hr))
		return FALSE;
	//SET_RENDER_DEBUG_NAME(depth_stencil_view, "Render_depth_stencil_view");

	xres = width;
	yres = height;

	ActivateBackbuffer();

	return TRUE;
}

// --------------------------------------
void
RenderManager::ActivateBackbuffer() 
{
	ctx->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)xres;
	vp.Height = (FLOAT)yres;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	ctx->RSSetViewports(1, &vp);
}

// --------------------------------------
void
RenderManager::Release() 
{
	if (ctx) ctx->ClearState();
	SAFE_RELEASE(depth_stencil_view);
	SAFE_RELEASE(depth_stencil);
	SAFE_RELEASE(render_target_view);
	SAFE_RELEASE(swap_chain);
	SAFE_RELEASE(ctx);
#ifdef _DEBUG
	ID3D11Debug *d3dDebug = nullptr;
	device->QueryInterface(__uuidof(ID3D11Debug), (void**)(&d3dDebug));
	d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
	SAFE_RELEASE(d3dDebug);
#endif
	SAFE_RELEASE(device); // ¿Por qué falla?
}