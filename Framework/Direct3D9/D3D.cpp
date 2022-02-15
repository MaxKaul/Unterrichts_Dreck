#include "D3D.h"
#include "Utils.h"

INT D3D::init(HWND hWnd, UINT width, UINT height, BOOL isFullscreen)
{
    // 1. get Direct3D 9 interface (connection to api)
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    CheckNull(pD3D, 20);

    // 2. check fixed-function pipeline support
    D3DCAPS9 d3dCaps = {};
    UINT adapter = D3DADAPTER_DEFAULT; // graphic card (0 = primary)
    D3DDEVTYPE devType = D3DDEVTYPE_HAL; // renderer (HAL = graphic card (Hardware Abstraction Layer))
    HRESULT hr = pD3D->GetDeviceCaps(adapter, devType, &d3dCaps);
    CheckFailed(hr, 22);

    DWORD vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING; // fallback
    if (d3dCaps.VertexProcessingCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
        vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING; // graphic card support the fixed-function pipeline

    // 3. set up presentation parameters
    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.hDeviceWindow = hWnd; // target window for rendering
    d3dpp.Windowed = !isFullscreen; // window mode or exclusive fullscreen
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // what should happen with front buffer after swapping with backbuffer
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; // (de-)activate vsync (default: activated)
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

    // 4. create Direct3D 9 device (connection to pipeline)
    hr = pD3D->CreateDevice(
        adapter,
        devType,
        hWnd, 
        vertexProcessing, // optional parameters, e.g. vertex processing
        &d3dpp,
        &_pD3DDevice
    );
    CheckFailed(hr, 24);
    
    // 5. tidy up
    safeRelease<IDirect3D9>(pD3D);

    return 0;
}

void D3D::beginScene(D3DCOLOR backgroundColor)
{
    // clear back buffer with solid color
    _pD3DDevice->Clear(
        0, nullptr, // regions to clear, 0 for whole buffer
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, // what to clear (bitmask), target -> back buffer
        backgroundColor, 1.0f, 0xffffffff // clear values, back buffer, depth buffer, stencil buffer
    );

    _pD3DDevice->BeginScene();
}

void D3D::endScene()
{
    _pD3DDevice->EndScene();

    // swap front with back buffer
    _pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

void D3D::deInit()
{
    safeRelease<IDirect3DDevice9>(_pD3DDevice);
}
