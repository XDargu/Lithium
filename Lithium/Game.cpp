#include "stdafx.h"
#include "Game.h"

static Game gGame;

Game& Game::GetInstance() {
	return gGame;
}

sBool
Game::Construct()
{
	return mRenderManager.Construct();
}

void
Game::Update()
{

}

void
Game::Render()
{
	float ClearColor[4] = { 0.1f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	mRenderManager.ctx->ClearRenderTargetView(mRenderManager.render_target_view, ClearColor);
	mRenderManager.ctx->ClearDepthStencilView(mRenderManager.depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

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