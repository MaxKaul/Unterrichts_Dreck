#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Mesh
{
public:
	INT init(ID3D11Device* pD3DDevice);
	void update(FLOAT dt);
	void render(ID3D11DeviceContext* pD3DDeviceContext);
	void deInit();

	XMFLOAT4X4* getWorldMatrix() { return &_worldMatrix; }

private:
	INT initVertexBuffer(ID3D11Device* pD3DDevice);
	INT initIndexBuffer(ID3D11Device* pD3DDevice);

	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;

	UINT _vertexCount = 0; // amount of vertices
	UINT _vertexStride = 0; // size of one vertex
	UINT _indexCount = 0; // amount of indices

	XMFLOAT4X4 _worldMatrix = {}; // world transformation matrix
};

