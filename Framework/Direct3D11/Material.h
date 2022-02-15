#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

class Material
{
public:
	INT init(ID3D11Device* pD3DDevice, LPCTSTR textureName);
	void render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* pWorldMatrix, XMFLOAT4X4* pViewMatrix, XMFLOAT4X4* pProjectionMatrix);
	void deInit();

private:
	INT createVertexShader(ID3D11Device* pD3DDevice);
	INT createPixelShader(ID3D11Device* pD3DDevice);
	INT createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pVertexShaderData); // exchange for flexible vertex format
	INT createMatrixBuffer(ID3D11Device* pD3DDevice);
	void setMatrices(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* pWorldMatrix, XMFLOAT4X4* pViewMatrix, XMFLOAT4X4* pProjectionMatrix);
	INT createTextureAndSampler(ID3D11Device* pD3DDevice, LPCTSTR textureName);

	ID3D11VertexShader* _pVertexShader = nullptr;
	ID3D11PixelShader* _pPixelShader = nullptr;
	ID3D11InputLayout* _pInputLayout = nullptr; // describe vertex layout

	ID3D11Buffer* _pMatrixBuffer = nullptr; // constant buffer for vertex shader
	struct MatrixBuffer
	{
		XMFLOAT4X4 worldViewProjectionMatrix;
		XMFLOAT4X4 worldMatrix;
	};

	ID3D11ShaderResourceView* _pTexture = nullptr;
	ID3D11SamplerState* _pSamplerState = nullptr;
	//D3DMATERIAL9 _material = {};
};

