#pragma once
#include <d3d9.h>

class Material
{
public:
	INT init(IDirect3DDevice9* pD3DDevice, LPCTSTR textureName);
	void render(IDirect3DDevice9* pD3DDevice);
	void deInit();

private:
	IDirect3DTexture9* _pTexture = nullptr;
	D3DMATERIAL9 _material = {};
};

