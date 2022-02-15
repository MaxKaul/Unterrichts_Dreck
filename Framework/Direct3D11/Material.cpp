#include "Material.h"
// https://github.com/microsoft/DirectXTex
#include "WICTextureLoader.h"
#include "Utils.h"
#include <d3dcompiler.h>

using namespace DirectX;

INT Material::init(ID3D11Device* pD3DDevice, LPCTSTR textureName)
{
	INT error = createVertexShader(pD3DDevice);
	CheckError(error);

	error = createPixelShader(pD3DDevice);
	CheckError(error);

	error = createMatrixBuffer(pD3DDevice);
	CheckError(error);

	error = createTextureAndSampler(pD3DDevice, textureName);
	CheckError(error);

	//_material.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	//_material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	//_material.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	//_material.Power = 1024.0f; // as greather the value is as smaller are the highlights
	//_material.Emissive = { 0.0f, 0.0f, 0.0f, 1.0f };

	return 0;
}

void Material::render(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* pWorldMatrix, XMFLOAT4X4* pViewMatrix, XMFLOAT4X4* pProjectionMatrix)
{
	// set texture and sampler state
	pD3DDeviceContext->PSSetShaderResources(0, 1, &_pTexture);
	pD3DDeviceContext->PSSetSamplers(0, 1, &_pSamplerState);

	//// set material
	//pD3DDevice->SetMaterial(&_material);

	// set shader pipeline
	pD3DDeviceContext->VSSetShader(_pVertexShader, nullptr, 0);
	pD3DDeviceContext->PSSetShader(_pPixelShader, nullptr, 0);
	pD3DDeviceContext->IASetInputLayout(_pInputLayout);

	// set matrices
	setMatrices(pD3DDeviceContext, pWorldMatrix, pViewMatrix, pProjectionMatrix);
}

void Material::deInit()
{
	safeRelease<ID3D11ShaderResourceView>(_pTexture);
	safeRelease<ID3D11SamplerState>(_pSamplerState);
	safeRelease<ID3D11Buffer>(_pMatrixBuffer);
	safeRelease<ID3D11InputLayout>(_pInputLayout);
	safeRelease<ID3D11VertexShader>(_pVertexShader);
	safeRelease<ID3D11PixelShader>(_pPixelShader);
}

INT Material::createVertexShader(ID3D11Device* pD3DDevice)
{
	ID3DBlob* pCompiledShaderCode = nullptr;

	// two options to load a shader (include d3dcompiler and load library)
	// 1. compile shader at runtime
	//HRESULT hr = D3DCompileFromFile(
	//	TEXT("ColorVertexShader.hlsl"), // hlsl shader file
	//	nullptr, // specify precompiler defines
	//	nullptr, // additional included shader code
	//	"main", // main function as entry point for shader
	//	"vs_4_0", // shader type and version
	//	0, 0, // additional flags
	//	&pCompiledShaderCode,
	//	nullptr // optional error messages
	//);
	//CheckFailed(hr, 60);

	// 2. load already compiled shader
	//HRESULT hr = D3DReadFileToBlob(TEXT("ColorVertexShader.cso"), &pCompiledShaderCode);
	//HRESULT hr = D3DReadFileToBlob(TEXT("TextureVertexShader.cso"), &pCompiledShaderCode);
	HRESULT hr = D3DReadFileToBlob(TEXT("LightingVertexShader.cso"), &pCompiledShaderCode);
	CheckFailed(hr, 60);

	// create shader object
	hr = pD3DDevice->CreateVertexShader(pCompiledShaderCode->GetBufferPointer(), pCompiledShaderCode->GetBufferSize(), nullptr, &_pVertexShader);
	CheckFailed(hr, 62);

	INT error = createInputLayout(pD3DDevice, pCompiledShaderCode);
	CheckError(error);

	safeRelease<ID3DBlob>(pCompiledShaderCode);

	return 0;
}

INT Material::createPixelShader(ID3D11Device* pD3DDevice)
{
	ID3DBlob* pCompiledShaderCode = nullptr;

	//HRESULT hr = D3DReadFileToBlob(TEXT("ColorPixelShader.cso"), &pCompiledShaderCode);
	//HRESULT hr = D3DReadFileToBlob(TEXT("TexturePixelShader.cso"), &pCompiledShaderCode);
	HRESULT hr = D3DReadFileToBlob(TEXT("LightingPixelShader.cso"), &pCompiledShaderCode);
	CheckFailed(hr, 64);

	hr = pD3DDevice->CreatePixelShader(pCompiledShaderCode->GetBufferPointer(), pCompiledShaderCode->GetBufferSize(), nullptr, &_pPixelShader);
	CheckFailed(hr, 66);

	safeRelease<ID3DBlob>(pCompiledShaderCode);

	return 0;
}

INT Material::createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pVertexShaderData)
{
	// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics
	
	//D3D11_INPUT_ELEMENT_DESC elements[] = {
	//	// position
	//	{
	//		"POSITION", // semantic name
	//		0, // semantic index
	//		DXGI_FORMAT_R32G32B32_FLOAT, // format
	//		0, // input slot
	//		0, // aligned byte offset
	//		D3D11_INPUT_PER_VERTEX_DATA, // input slot class
	//		0 // instance data step rate
	//	}
	//};

	D3D11_INPUT_ELEMENT_DESC elements[4] = {};

	// position
	elements[0].SemanticName = "POSITION";
	elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	// normal
	elements[1].SemanticName = "NORMAL";
	elements[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// uv
	elements[2].SemanticName = "TEXCOORD";
	elements[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	elements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// color
	elements[3].SemanticName = "COLOR";
	elements[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	HRESULT hr = pD3DDevice->CreateInputLayout(elements, 4, pVertexShaderData->GetBufferPointer(), pVertexShaderData->GetBufferSize(), &_pInputLayout);
	CheckFailed(hr, 68);

	return 0;
}

INT Material::createMatrixBuffer(ID3D11Device* pD3DDevice)
{
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(MatrixBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, nullptr, &_pMatrixBuffer);
	CheckFailed(hr, 61);

	return 0;
}

void Material::setMatrices(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* pWorldMatrix, XMFLOAT4X4* pViewMatrix, XMFLOAT4X4* pProjectionMatrix)
{
	XMMATRIX worldMatrix = XMLoadFloat4x4(pWorldMatrix);
	XMMATRIX viewMatrix = XMLoadFloat4x4(pViewMatrix);
	XMMATRIX projectionMatrix = XMLoadFloat4x4(pProjectionMatrix);

	XMMATRIX wvpMatrix = XMMatrixTranspose(worldMatrix * viewMatrix * projectionMatrix); // transpose because of column mayor - row mayor problem

	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map(_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	if (FAILED(hr)) return;

	MatrixBuffer* pMatrixBuffer = static_cast<MatrixBuffer*>(data.pData);
	XMStoreFloat4x4(&(pMatrixBuffer->worldViewProjectionMatrix), wvpMatrix);
	XMStoreFloat4x4(&(pMatrixBuffer->worldMatrix), XMMatrixTranspose(worldMatrix));

	pD3DDeviceContext->Unmap(_pMatrixBuffer, 0);

	pD3DDeviceContext->VSSetConstantBuffers(0, 1, &_pMatrixBuffer);
}

INT Material::createTextureAndSampler(ID3D11Device* pD3DDevice, LPCTSTR textureName)
{
	// create texture
	HRESULT hr = CreateWICTextureFromFile(pD3DDevice, textureName, nullptr, &_pTexture);
	CheckFailed(hr, 63);

	// create sampler state
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	hr = pD3DDevice->CreateSamplerState(&desc, &_pSamplerState);
	CheckFailed(hr, 65);
	
	return 0;
}
