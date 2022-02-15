#include "Light.h"

INT Light::init(D3DLIGHT9& light, INT id)
{
    _light = light;
    _id = id;

    return 0;
}

void Light::render(IDirect3DDevice9* pD3DDevice)
{
    pD3DDevice->SetLight(_id, &_light);
    pD3DDevice->LightEnable(_id, TRUE);
}

void Light::deInit()
{
}
