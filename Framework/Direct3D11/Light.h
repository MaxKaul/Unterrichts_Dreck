#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Light
{
public:

	struct LightData
	{
		//XMFLOAT3 lightDirection;
		//FLOAT padding; // because of 16 byte alignment
		//XMFLOAT4 lightDiffuseColor;
		//FLOAT lightIntensity;
		//XMFLOAT3 padding2; // because of 16 byte alignment

		XMFLOAT3 lightDirection;
		FLOAT lightIntensity;
		XMFLOAT4 lightDiffuseColor;
	};

	INT init(ID3D11Device* pD3DDevice, LightData &light, INT id = 0);
	void render(ID3D11DeviceContext* pD3DDeviceContext);
	void deInit();

private:
	INT _id = 0; // light slot
	LightData _light = {}; // light information
	ID3D11Buffer* _pLightBuffer = nullptr;
};

