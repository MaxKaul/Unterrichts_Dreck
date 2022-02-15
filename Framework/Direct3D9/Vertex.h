#pragma once
#include <d3d9.h>

#define FVF D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL

struct Vertex
{
	// position
	FLOAT _x;
	FLOAT _y;
	FLOAT _z;

	// normal 
	FLOAT _nx;
	FLOAT _ny;
	FLOAT _nz;

	// color
	D3DCOLOR _color;

	// uv
	FLOAT _u;
	FLOAT _v;

	Vertex(FLOAT x, FLOAT y, FLOAT z) : _x(x), _y(y), _z(z), _color(0xffffffff) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, D3DCOLOR color) : _x(x), _y(y), _z(z), _color(color) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, UINT r, UINT g, UINT b) : _x(x), _y(y), _z(z), _color(D3DCOLOR_XRGB(r, g, b)) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v) : _x(x), _y(y), _z(z), _color(0xffffffff), _u(u), _v(v) {}
	Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT nx, FLOAT ny, FLOAT nz, FLOAT u, FLOAT v) : _x(x), _y(y), _z(z), _nx(nx), _ny(ny), _nz(nz), _color(0xffffffff), _u(u), _v(v) {}
};