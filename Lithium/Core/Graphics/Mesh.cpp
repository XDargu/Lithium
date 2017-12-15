#include "stdafx.h"
#include "Mesh.h"
#include "Game.h"


cEgMesh::cEgMesh()
    : mVertexBuffer(nullptr)
    , mIndexBuffer(nullptr)
    , muNumVertices(0)
    , muNumIndices(0)
    , mTopology(D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED)
    , mVertexDecl(nullptr)
{ }

cEgMesh::~cEgMesh()
{
}

sBool
cEgMesh::Create(
    sUInt32              luNumVertices, 
    const void *         lpVertices, 
    sUInt32              luNumIndices, 
    const TIndex *       lpIndices, 
    ePrimitiveType       lePrimitiveType, 
    const cEgVertexDecl* lVertexDecl, 
    sBool                lbMutable)
{
    POW2_ASSERT_MSG(mVertexBuffer == nullptr, "Mesh already created");

    // Check Data
    POW2_ASSERT_MSG(luNumVertices > 0, "Invalid num vertices for mesh creation");
    POW2_ASSERT_MSG(lpVertices != nullptr, "Invalid vertices for mesh creation");
    POW2_ASSERT_MSG(lVertexDecl != nullptr, "Invalid vertex declaration for mesh creation");
    POW2_ASSERT_MSG(lVertexDecl->muBytesPerVertex > 0, "Invalid vertex declaration bytes format for mesh creation");

    muNumVertices = luNumVertices;
    muNumIndices = luNumIndices;
    mVertexDecl = lVertexDecl;

    switch (lePrimitiveType) {
        case POINTS:         mTopology = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST; break;
        case LINE_LIST:      mTopology = D3D10_PRIMITIVE_TOPOLOGY_LINELIST; break;
        case TRIANGLE_LIST:  mTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
        case TRIANGLE_STRIP: mTopology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
        default:
            POW2_ASSERT_FAIL("Primitive_type %d is not valid", lePrimitiveType);
    }

    // Create the VB
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = lVertexDecl->muBytesPerVertex * muNumVertices;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    if (lbMutable) {
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.Usage = D3D11_USAGE_DYNAMIC;
    }

    // The initial contents of the VB
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = lpVertices;
    HRESULT hr = Game::GetInstance().mRenderManager.device->CreateBuffer(&bd, &InitData, &mVertexBuffer);
    if (FAILED(hr))
        return false;

    // Create the Index Buffer if the user gives us indices
    if (lpIndices) {
        POW2_ASSERT_MSG(muNumIndices > 0, "Invalid num indices for mesh creation");

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(TIndex) * muNumIndices;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        InitData.pSysMem = lpIndices;
        hr = Game::GetInstance().mRenderManager.device->CreateBuffer(&bd, &InitData, &mIndexBuffer);
        if (FAILED(hr))
            return false;
    }
    else {
        POW2_ASSERT_MSG(muNumIndices == 0, "Number of indices must be 0 in mesh creation if no indices are provided");
    }

    return TRUE;
}

// --------------------------------------
void
cEgMesh::UpdateFromCPU(
    const void *    lpNewCPUData, 
    size_t          luNumBytesToUpdate)
{
    if (luNumBytesToUpdate == 0)
        luNumBytesToUpdate = muNumVertices * mVertexDecl->muBytesPerVertex;
    
    POW2_ASSERT_MSG(mVertexBuffer, "Invalid vertex buffer");

    D3D11_MAPPED_SUBRESOURCE lMappedResource;

    // Get CPU access to the GPU buffer
    HRESULT hr = Game::GetInstance().mRenderManager.ctx->Map(
        mVertexBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &lMappedResource);

    POW2_ASSERT(hr == D3D_OK);
    POW2_ASSERT(luNumBytesToUpdate <= mVertexDecl->muBytesPerVertex * muNumVertices);

    // Copy from CPU to GPU
    memcpy(lMappedResource.pData, lpNewCPUData, luNumBytesToUpdate);

    // Close the map
    Game::GetInstance().mRenderManager.ctx->Unmap(mVertexBuffer, 0);

}

void
cEgMesh::Activate() const 
{
    POW2_ASSERT_MSG(mVertexBuffer, "Error loading vertex buffer");

    // Activate the vertex buffer
    UINT stride = mVertexDecl->muBytesPerVertex;
    UINT offset = 0;
    Game::GetInstance().mRenderManager.ctx->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

    // Set primitive topology (LINES,POINTS,..)
//#ifdef _DEBUG
    //Game::GetInstance().mRenderManager.ctx->IASetPrimitiveTopology(mTopology == D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST ? D3D10_PRIMITIVE_TOPOLOGY_LINELIST : mTopology);
//#else
    Game::GetInstance().mRenderManager.ctx->IASetPrimitiveTopology(mTopology);
//#endif

    // Set index buffer
    if (mIndexBuffer)
        Game::GetInstance().mRenderManager.ctx->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    //current_active_mesh = this;
}

// --------------------------------------
void 
cEgMesh::Render() const 
{
    //POW2_ASSERT(current_active_mesh == this);  // Did you forget to activate this mesh?

    POW2_ASSERT(mVertexBuffer);
    if (mIndexBuffer)
        Game::GetInstance().mRenderManager.ctx->DrawIndexed(muNumIndices, 0, 0);
    else
        Game::GetInstance().mRenderManager.ctx->Draw(muNumVertices, 0);
}