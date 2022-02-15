#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;

INT Camera::init(UINT screenWidth, UINT screenHeight)
{
    // view matrix
    XMMATRIX viewMatrix = XMMatrixLookToLH(
        XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), // camera position
        XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), // camera forward vector
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) // camera up vector
    );
    XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&_viewMatrix), viewMatrix);

    // projection matrix
    XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(
        XM_PI * 0.3333333f, // field of view in radians
        static_cast<FLOAT>(screenWidth) / static_cast<FLOAT>(screenHeight), // aspect ratio
        0.3f, 1000.0f // near and far clipping plane
    );
    XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&_projectionMatrix), projectionMatrix);

    return 0;
}

void Camera::render(IDirect3DDevice9* pD3DDevice)
{
    pD3DDevice->SetTransform(D3DTS_VIEW, &_viewMatrix);
    pD3DDevice->SetTransform(D3DTS_PROJECTION, &_projectionMatrix);
}

void Camera::deInit()
{
}
