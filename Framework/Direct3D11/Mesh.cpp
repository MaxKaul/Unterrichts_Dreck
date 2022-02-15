#include "Mesh.h"
#include "Utils.h"
#include "Vertex.h"
#include <xutility>

INT Mesh::init(ID3D11Device* pD3DDevice)
{
	INT error = initVertexBuffer(pD3DDevice);
	CheckError(error);

	error = initIndexBuffer(pD3DDevice);
	CheckError(error);

	// initialize world transformation matrix
	XMStoreFloat4x4(&_worldMatrix, XMMatrixIdentity());

	return 0;
}

void Mesh::update(FLOAT dt)
{
	static FLOAT posX = 0.0f;
	static FLOAT posY = 0.0f;
	static FLOAT posZ = 0.0f;
	static FLOAT rot = 0.0f;
	rot += XM_PI / 3.0f * dt;

	FLOAT move = 5.0f * dt;

	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000)) posX -= move;
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) || (GetAsyncKeyState('D') & 0x8000)) posX += move;
	if ((GetAsyncKeyState(VK_UP) & 0x8000) || (GetAsyncKeyState('W') & 0x8000)) posY += move;
	if ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState('S') & 0x8000)) posY -= move;
	if ((GetAsyncKeyState(VK_SUBTRACT) & 0x8000) || (GetAsyncKeyState('Q') & 0x8000)) posZ -= move;
	if ((GetAsyncKeyState(VK_ADD) & 0x8000) || (GetAsyncKeyState('E') & 0x8000)) posZ += move;

	XMMATRIX translation = XMMatrixTranslation(posX, posY, posZ);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(0.0f, rot, 0.0f);
	XMMATRIX localScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	XMStoreFloat4x4(&_worldMatrix, localScale * rotation * translation);
}

void Mesh::render(ID3D11DeviceContext* pD3DDeviceContext)
{
	// set mesh data
	static UINT offset = 0;
	pD3DDeviceContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &_vertexStride, &offset);
	pD3DDeviceContext->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, offset);
	pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw with index buffer
	pD3DDeviceContext->DrawIndexed(_indexCount, 0, 0);
}

void Mesh::deInit()
{
	safeRelease<ID3D11Buffer>(_pVertexBuffer);
	safeRelease<ID3D11Buffer>(_pIndexBuffer);
}

INT Mesh::initVertexBuffer(ID3D11Device* pD3DDevice)
{
	Vertex vertices[] = {
		//// quad with color
		//Vertex(-0.5f, 0.5f, 0.0f, 255u, 0u, 0u),
		//Vertex(0.5f, 0.5f, 0.0f, 0u, 255u, 0u),
		//Vertex(0.5f, -0.5f, 0.0f, 255u, 0u, 255u),
		//Vertex(-0.5f, -0.5f, 0.0f, 0u, 0u, 255u)

		// quad with uv & normal
		Vertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
		Vertex(0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
		Vertex(0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
		Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f)	
	};

	_vertexCount = std::size(vertices);
	_vertexStride = sizeof(Vertex);

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = _vertexCount * _vertexStride;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // buffer type
	desc.Usage = D3D11_USAGE_IMMUTABLE; // who has which access

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = vertices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pVertexBuffer);
	CheckFailed(hr, 30);

	return 0;
}

INT Mesh::initIndexBuffer(ID3D11Device* pD3DDevice)
{
	USHORT indices[] = {
		// quad with 2 triangles
		// primitive 1
		0, 1, 2,

		// primitive 2
		0, 2, 3
	};

	_indexCount = std::size(indices);

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = _indexCount * sizeof(USHORT);
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = indices;

	HRESULT hr = pD3DDevice->CreateBuffer(&desc, &initialData, &_pIndexBuffer);
	CheckFailed(hr, 32);

	return 0;
}
