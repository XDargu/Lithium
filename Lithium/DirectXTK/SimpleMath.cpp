//-------------------------------------------------------------------------------------
// SimpleMath.cpp -- Simplified C++ Math wrapper for DirectXMath
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//-------------------------------------------------------------------------------------

#include "stdafx.h"
#include <algorithm>
#include "SimpleMath.h"

/****************************************************************************
*
* Constants
*
****************************************************************************/

namespace DirectX
{
    namespace SimpleMath
    {
#if defined(_MSC_VER) && (_MSC_VER < 1800)
        const Vector2 Vector2::Zero(0.f, 0.f);
        const Vector2 Vector2::One(1.f, 1.f);
        const Vector2 Vector2::UnitX(1.f, 0.f);
        const Vector2 Vector2::UnitY(0.f, 1.f);

        const Vector3 Vector3::Zero(0.f, 0.f, 0.f);
        const Vector3 Vector3::One(1.f, 1.f, 1.f);
        const Vector3 Vector3::UnitX(1.f, 0.f, 0.f);
        const Vector3 Vector3::UnitY(0.f, 1.f, 0.f);
        const Vector3 Vector3::UnitZ(0.f, 0.f, 1.f);
        const Vector3 Vector3::Up(0.f, 1.f, 0.f);
        const Vector3 Vector3::Down(0.f, -1.f, 0.f);
        const Vector3 Vector3::Right(1.f, 0.f, 0.f);
        const Vector3 Vector3::Left(-1.f, 0.f, 0.f);
        const Vector3 Vector3::Forward(0.f, 0.f, -1.f);
        const Vector3 Vector3::Backward(0.f, 0.f, 1.f);

        const Vector4 Vector4::Zero(0.f, 0.f, 0.f, 0.f);
        const Vector4 Vector4::One(1.f, 1.f, 1.f, 1.f);
        const Vector4 Vector4::UnitX(1.f, 0.f, 0.f, 0.f);
        const Vector4 Vector4::UnitY(0.f, 1.f, 0.f, 0.f);
        const Vector4 Vector4::UnitZ(0.f, 0.f, 1.f, 0.f);
        const Vector4 Vector4::UnitW(0.f, 0.f, 0.f, 1.f);

        const Matrix Matrix::Identity(1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f);

        const Quaternion Quaternion::Identity(0.f, 0.f, 0.f, 1.f);
#else
        const Vector2 Vector2::Zero = { 0.f, 0.f };
        const Vector2 Vector2::One = { 1.f, 1.f };
        const Vector2 Vector2::UnitX = { 1.f, 0.f };
        const Vector2 Vector2::UnitY = { 0.f, 1.f };

        const Vector3 Vector3::Zero = { 0.f, 0.f, 0.f };
        const Vector3 Vector3::One = { 1.f, 1.f, 1.f };
        const Vector3 Vector3::UnitX = { 1.f, 0.f, 0.f };
        const Vector3 Vector3::UnitY = { 0.f, 1.f, 0.f };
        const Vector3 Vector3::UnitZ = { 0.f, 0.f, 1.f };
        const Vector3 Vector3::Up = { 0.f, 1.f, 0.f };
        const Vector3 Vector3::Down = { 0.f, -1.f, 0.f };
        const Vector3 Vector3::Right = { 1.f, 0.f, 0.f };
        const Vector3 Vector3::Left = { -1.f, 0.f, 0.f };
        const Vector3 Vector3::Forward = { 0.f, 0.f, -1.f };
        const Vector3 Vector3::Backward = { 0.f, 0.f, 1.f };

        const Vector4 Vector4::Zero = { 0.f, 0.f, 0.f, 0.f };
        const Vector4 Vector4::One = { 1.f, 1.f, 1.f, 1.f };
        const Vector4 Vector4::UnitX = { 1.f, 0.f, 0.f, 0.f };
        const Vector4 Vector4::UnitY = { 0.f, 1.f, 0.f, 0.f };
        const Vector4 Vector4::UnitZ = { 0.f, 0.f, 1.f, 0.f };
        const Vector4 Vector4::UnitW = { 0.f, 0.f, 0.f, 1.f };

        const Matrix Matrix::Identity = { 1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f };

        const Quaternion Quaternion::Identity = { 0.f, 0.f, 0.f, 1.f };
#endif
    }
}


/****************************************************************************
*
* Viewport
*
****************************************************************************/

#if defined(__d3d11_h__) || defined(__d3d11_x_h__)
static_assert(sizeof(DirectX::SimpleMath::Viewport) == sizeof(D3D11_VIEWPORT), "Size mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, x) == FIELD_OFFSET(D3D11_VIEWPORT, TopLeftX), "Layout mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, y) == FIELD_OFFSET(D3D11_VIEWPORT, TopLeftY), "Layout mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, width) == FIELD_OFFSET(D3D11_VIEWPORT, Width), "Layout mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, height) == FIELD_OFFSET(D3D11_VIEWPORT, Height), "Layout mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, minDepth) == FIELD_OFFSET(D3D11_VIEWPORT, MinDepth), "Layout mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, maxDepth) == FIELD_OFFSET(D3D11_VIEWPORT, MaxDepth), "Layout mismatch");
#endif

#if defined(__d3d12_h__) || defined(__d3d12_x_h__)
static_assert(sizeof(DirectX::SimpleMath::Viewport) == sizeof(D3D12_VIEWPORT), "Size mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, x) == FIELD_OFFSET(D3D12_VIEWPORT, TopLeftX), "Layout mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, y) == FIELD_OFFSET(D3D12_VIEWPORT, TopLeftY), "Layout mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, width) == FIELD_OFFSET(D3D12_VIEWPORT, Width), "Layout mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, height) == FIELD_OFFSET(D3D12_VIEWPORT, Height), "Layout mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, minDepth) == FIELD_OFFSET(D3D12_VIEWPORT, MinDepth), "Layout mismatch");
static_assert(FIELD_OFFSET(DirectX::SimpleMath::Viewport, maxDepth) == FIELD_OFFSET(D3D12_VIEWPORT, MaxDepth), "Layout mismatch");
#endif

RECT DirectX::SimpleMath::Viewport::ComputeTitleSafeArea(UINT backBufferWidth, UINT backBufferHeight)
{
    float safew = (float(backBufferWidth) + 19.f) / 20.f;
    float safeh = (float(backBufferHeight) + 19.f) / 20.f;

    RECT rct;
    rct.left = static_cast<LONG>(safew);
    rct.top = static_cast<LONG>(safeh);
    rct.right = static_cast<LONG>(float(backBufferWidth) - safew + 0.5f);
    rct.bottom = static_cast<LONG>(float(backBufferHeight) - safeh + 0.5f);

    return rct;
}