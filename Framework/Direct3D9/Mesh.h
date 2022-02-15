#pragma once
#include <d3d9.h>

class Mesh
{
public:
	INT init(IDirect3DDevice9* pD3DDevice);
	void update(FLOAT dt);
	void render(IDirect3DDevice9* pD3DDevice);
	void deInit();

private:
	INT initVertexBuffer(IDirect3DDevice9* pD3DDevice);
	INT initIndexBuffer(IDirect3DDevice9* pD3DDevice);

	IDirect3DVertexBuffer9* _pVertexBuffer = nullptr;
	IDirect3DIndexBuffer9* _pIndexBuffer = nullptr;

	INT _vertexCount = 0; // amount of vertices
	INT _vertexStride = 0; // size of one vertex
	INT _indexCount = 0; // amount of indices

	D3DMATRIX _worldMatrix = {}; // world transformation matrix
};

