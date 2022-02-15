#include "Light.h"
#include "Utils.h"

INT Light::init(ID3D11Device* pD3DDevice, LightData& light, INT id)
{
    _light = light;
    _id = id;

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(LightData);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.Usage = D3D11_USAGE_IMMUTABLE;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = &light;

    HRESULT hr = pD3DDevice->CreateBuffer(&desc, &data, &_pLightBuffer);
    CheckFailed(hr, 70);

    return 0;
}

void Light::render(ID3D11DeviceContext* pD3DDeviceContext)
{
    pD3DDeviceContext->PSSetConstantBuffers(_id, 1, &_pLightBuffer);
}

void Light::deInit()
{
    safeRelease<ID3D11Buffer>(_pLightBuffer);
}
