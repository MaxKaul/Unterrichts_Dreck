#pragma once
#include <Windows.h>

class Window
{
public:
	INT init(HINSTANCE hInstance, UINT width, UINT height);
	BOOL run();
	void deInit();

	HWND getWindowHandle() { return _hWnd; }

private:
	HWND _hWnd = nullptr;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);