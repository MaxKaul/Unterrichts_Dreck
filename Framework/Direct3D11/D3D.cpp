#include "D3D.h"
#include "Utils.h"
#include <xutility>

INT D3D::init(HWND hWnd, UINT width, UINT height, BOOL isFullscreen)
{
    HRESULT hr;

    // 1. describe creation process
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = !isFullscreen;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.SampleDesc.Count = 1; // this MSAA count have to be set to at least 1

    // 2. create Direct3D 11 device, device context & swap chain
    D3D_FEATURE_LEVEL supportedFeatureLevels[] = {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_FEATURE_LEVEL choosenFeatureLevel = {};

    hr = D3D11CreateDeviceAndSwapChain(
        nullptr, // use primary graphic card
        D3D_DRIVER_TYPE_HARDWARE, nullptr, // equivalent top d3d9 dev type
        0, // flags
        supportedFeatureLevels, std::size(supportedFeatureLevels), // which Direct3D version will be supported by our application
        D3D11_SDK_VERSION,
        &swapChainDesc, &_pDXGISwapChain, &_pD3DDevice, &choosenFeatureLevel, &_pD3DDeviceContext
    );
    CheckFailed(hr, 20);
    
    // 3. create render target view
    ID3D11Texture2D* pBackBufferTexture = nullptr;
    //hr = _pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBufferTexture));
    hr = _pDXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBufferTexture));
    CheckFailed(hr, 22);

    hr = _pD3DDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &_pRenderTargetView);
    CheckFailed(hr, 24);

    safeRelease<ID3D11Texture2D>(pBackBufferTexture);

    // 4. create depth stencil view 
    ID3D11Texture2D* pDepthStencilTexture = nullptr;
    D3D11_TEXTURE2D_DESC depthStencilTextureDesc = {};
    depthStencilTextureDesc.Width = width;
    depthStencilTextureDesc.Height = height;
    depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilTextureDesc.ArraySize = 1; // at least one texture
    depthStencilTextureDesc.SampleDesc.Count = 1; // should be always one

    hr = _pD3DDevice->CreateTexture2D(&depthStencilTextureDesc, nullptr, &pDepthStencilTexture);
    CheckFailed(hr, 26);

    hr = _pD3DDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &_pDepthStencilView);
    CheckFailed(hr, 28);

    safeRelease<ID3D11Texture2D>(pDepthStencilTexture);

    // 5. create rasterizer state
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;

    hr = _pD3DDevice->CreateRasterizerState(&rasterizerDesc, &_pRasterizerState);
    CheckFailed(hr, 29);

    // 6. set viewport
    D3D11_VIEWPORT viewPort = {}; // describe area projected onto screen/window
    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;
    viewPort.Width = width;
    viewPort.Height = height;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;

    // 7. set up render pipeline
    _pD3DDeviceContext->OMSetRenderTargets(1, &_pRenderTargetView, _pDepthStencilView);
    _pD3DDeviceContext->RSSetViewports(1, &viewPort);
    _pD3DDeviceContext->RSSetState(_pRasterizerState);

    return 0;
}

void D3D::beginScene(FLOAT red, FLOAT green, FLOAT blue)
{
    // clear back buffer
    FLOAT backgroundColor[] = { red, green, blue, 1.0f };
    _pD3DDeviceContext->ClearRenderTargetView(_pRenderTargetView, backgroundColor);

    // clear depth buffer
    _pD3DDeviceContext->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0xffffffff);
}

void D3D::endScene()
{
    // swap front with back buffer
    _pDXGISwapChain->Present(0, 0);
}

void D3D::deInit()
{
    safeRelease<ID3D11RasterizerState>(_pRasterizerState);
    safeRelease<ID3D11DepthStencilView>(_pDepthStencilView);
    safeRelease<ID3D11RenderTargetView>(_pRenderTargetView);
    safeRelease<ID3D11Device>(_pD3DDevice);
    safeRelease<ID3D11DeviceContext>(_pD3DDeviceContext);
    safeRelease<IDXGISwapChain>(_pDXGISwapChain);
}
