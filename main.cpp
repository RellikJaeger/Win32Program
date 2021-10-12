// Definations for compiler flags
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef UNICODE // IMPORTANT
#define UNICODE // IMPORTANT
#endif          // IMPORTANT

#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef WX
#define WX
#endif

// Required libs for linker
#pragma comment(lib, "user32") // IMPORTANT
#pragma comment(lib, "gdi32")
#pragma comment(lib, "uxtheme")

// C Headers
#include <windows.h> // IMPORTANT

// C++ classes
#include <iostream>
#include <string>
#include <vector>

// Identifiers/declarations
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // IMPORTANT

void CenterWindow(HWND hwnd);

// Windows API entry for win32 program
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) { // IMPORTANT
	MSG  msg; // IMPORTANT
	HWND hwnd; // IMPORTANT
	WNDCLASSW wc; // IMPORTANT

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.lpszClassName = L"Main Window"; // IMPORTANT
	wc.hInstance     = hInstance; // IMPORTANT
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName  = NULL;
	wc.lpfnWndProc   = WndProc; // IMPORTANT
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // IMPORTANT
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	
	RegisterClassW(&wc); // IMPORTANT

	hwnd = CreateWindowW(wc.lpszClassName, L"Anime Connect", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 600, 400, NULL, NULL, hInstance, NULL); // IMPORTANT

	ShowWindow(hwnd, nCmdShow); // IMPORTANT
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) { // IMPORTANT
		TranslateMessage(&msg);
		DispatchMessage(&msg); // IMPORTANT
	}

	return (int) msg.wParam;
}

// Window process callback to handle process events/messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { // IMPORTANT
	switch (uMsg) {
		case WM_CREATE:
			CenterWindow(hwnd);
			break;

		case WM_DESTROY: // IMPORTANT
			PostQuitMessage(0); // IMPORTANT
			break;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
				EndPaint(hwnd, &ps);
			}
			break;

		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam); // IMPORTANT
}

// CenterWindow function
void CenterWindow(HWND hwnd) {
	RECT rc = {0};

	GetWindowRect(hwnd, &rc);
	int win_w = rc.right - rc.left;
	int win_h = rc.bottom - rc.top;

	int screen_w = GetSystemMetrics(SM_CXSCREEN);
	int screen_h = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(hwnd, HWND_TOP, (screen_w - win_w) / 2, 
		(screen_h - win_h) / 2, 0, 0, SWP_NOSIZE);
}
