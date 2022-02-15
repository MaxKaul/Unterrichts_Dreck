#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
	XMFLOAT4 color;

	Vertex(FLOAT x, FLOAT y, FLOAT z) : position(x, y, z), normal(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT r, FLOAT g, FLOAT b) : position(x, y, z), normal(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f), color(r, g, b, 1.0f) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, UINT r, UINT g, UINT b) : position(x, y, z), normal(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f), color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v) : position(x, y, z), normal(0.0f, 0.0f, 0.0f), uv(u, v), color(1.0f, 1.0f, 1.0f, 1.0f) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT nx, FLOAT ny, FLOAT nz, FLOAT u, FLOAT v) : position(x, y, z), normal(nx, ny, nz), uv(u, v), color(1.0f, 1.0f, 1.0f, 1.0f) {}
};