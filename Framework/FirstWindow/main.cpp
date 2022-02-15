#include <Windows.h>

LRESULT CALLBACK WndProc(
	HWND hWnd, // handle to window instance this message is for
	UINT msg, // message id, e.g. WM_CLOSE
	WPARAM wParam, // main message information (single information)
	LPARAM lParam // additional message informations (multiple informations)
);

int WINAPI WinMain(
	HINSTANCE hInstance, // handle to application instance
	HINSTANCE hPrevInstance, // deprecated
	LPSTR szCmdLine, // command line
	int nCmdShow // how the user want to start the window (minimised, maximised or normal window)
)
{
	UINT width = 1024;
	UINT height = 780;

	// 1. describe window class
	WNDCLASS wc = {};
	wc.hInstance = hInstance; // handle to application instance
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0)); // handle to background brush (color)
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW); // handle to application cursor
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // handle to application icon
	wc.lpszClassName = TEXT("First Window"); // window class name
	wc.lpfnWndProc = WndProc; // communication interface between Windows and application
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // additional window behaviour

	// 2. register window class
	if (RegisterClass(&wc) == 0) return 10;

	// 3. adjust window size (optional)
	UINT screenWidth = GetSystemMetrics(SM_CXSCREEN);
	UINT screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// centered window
	RECT wr = { (screenWidth - width) / 2, (screenHeight - height) / 2, (screenWidth + width) / 2, (screenHeight + height) / 2 };
	DWORD style = WS_OVERLAPPEDWINDOW;

	// windowed fullscreen
	//RECT wr = { 0, 0, screenWidth, screenHeight };
	//DWORD style = WS_POPUP;

	AdjustWindowRect(&wr, style, false);

	// 4. instantiate window
	HWND hWnd = CreateWindow(
		wc.lpszClassName, // registered window class name
		wc.lpszClassName, // window title
		style, // visual window style
		wr.left, wr.top, // left-top corner of window
		wr.right - wr.left, wr.bottom - wr.top, // width and height of window
		nullptr, // handle to parent window
		nullptr, // handle to menu instance
		hInstance, // handle tot application instance
		nullptr // optional parameters
	);

	if (hWnd == nullptr) return 12;

	// 5. show window and set focus
	ShowWindow(hWnd, nCmdShow);
	SetFocus(hWnd);

	// 6. message loop
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		// GetMessage - returns next message, but it will wait until next message is available
		// PeekMessage - returns a message if one is available or nothing
		// TranslateMessage - (optional) translates keyboard keys
		// DispatchMessage - sends the message to the WindowProc

		if (PeekMessage(&msg, nullptr, 0, UINT_MAX, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Sleep(10);
	}

	return msg.wParam; // return wm quit exit code
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}
