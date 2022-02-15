#pragma once
#include <d3d9.h>

class Light
{
public:
	INT init(D3DLIGHT9 &light, INT id = 0);
	void render(IDirect3DDevice9* pD3DDevice);
	void deInit();

private:
	INT _id = 0; // light slot
	D3DLIGHT9 _light = {}; // light information
};

