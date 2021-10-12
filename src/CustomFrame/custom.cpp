// Definitions for compiler flags
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
#pragma comment(lib, "dwmapi")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "uxtheme")

// C Headers
#include <windows.h> // IMPORTANT
#include <windowsx.h> // for `GET_X_LPARAM` etc.
#include <dwmapi.h>

// C++ classes
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

const auto s_brush = CreateSolidBrush(RGB(0, 0, 255));

// Identifiers/declarations
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // IMPORTANT

void CenterWindow(HWND hwnd);

// Windows API entry for win32 program
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) { // IMPORTANT
	// MSG  uMsg; // IMPORTANT
	// HWND hwnd; // IMPORTANT
	// WNDCLASSW wc; // IMPORTANT

	// wc.style         = CS_HREDRAW | CS_VREDRAW;
	// wc.cbClsExtra    = 0;
	// wc.cbWndExtra    = 0;
	// wc.lpszClassName = L"Main Window"; // IMPORTANT
	// wc.hInstance     = hInstance; // IMPORTANT
	// wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	// wc.lpszMenuName  = NULL;
	// wc.lpfnWndProc   = WndProc; // IMPORTANT
	// wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // IMPORTANT
	// wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	
	// RegisterClassW(&wc); // IMPORTANT

	// hwnd = CreateWindowW(wc.lpszClassName, L"Anime Connect", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 600, 400, NULL, NULL, hInstance, NULL); // IMPORTANT

	// ShowWindow(hwnd, nCmdShow); // IMPORTANT
	// UpdateWindow(hwnd);

	// while (GetMessage(&uMsg, NULL, 0, 0)) { // IMPORTANT
	// 	TranslateMessage(&uMsg);
	// 	DispatchMessage(&uMsg); // IMPORTANT
	// }

	// return (int) uMsg.wParam;

	WNDCLASS wc{ CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance, 0, 
		LoadCursor(NULL, IDC_ARROW), 
		reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)), 0, L"MainWindow" };

		const auto hwnd = CreateWindow(MAKEINTATOM(RegisterClass(&wc)), L"Anime Connect", WS_OVERLAPPEDWINDOW,
			0, 0, 600, 400, 0, 0, hInstance, 0);

		ShowWindow(hwnd, nCmdShow);
		UpdateWindow(hwnd);

		MSG uMsg;

		while (GetMessageW(&uMsg, 0, 0, 0))
		{
			TranslateMessage(&uMsg);
			DispatchMessageW(&uMsg);
		}

		return 0;
	}

// Window process callback to handle process events/messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { // IMPORTANT
	static int cy_titlebar_ = 30;
	static RECT border_thickness;

	LRESULT result;

	if (DwmDefWindowProc(hwnd, uMsg, wParam, lParam, &result)) {
		return result;
	}

	switch (uMsg) {
		case WM_CREATE: {
			CenterWindow(hwnd);
			//find border thickness
			border_thickness = {0};
			if(GetWindowLongPtr(hwnd, GWL_STYLE) & WS_THICKFRAME) {
				AdjustWindowRectEx(&border_thickness,
				GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_CAPTION, FALSE, NULL);
				border_thickness.left *= -1;
				border_thickness.top *= -1;
			} else if(GetWindowLongPtr(hwnd, GWL_STYLE) & WS_BORDER) {
				border_thickness = { 1,1,1,1 };
			}

			MARGINS margins = { 0, 0, cy_titlebar_, 0 };
			DwmExtendFrameIntoClientArea(hwnd, &margins);
			SetWindowPos(hwnd, NULL, 0, 0, 0, 0, 
				SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
			return 0;
		}
		break;

		case WM_ACTIVATE:
		{
			int metrics[4];
			const auto window_dpi_ = GetDpiForWindow(hwnd);

			metrics[0] = GetSystemMetricsForDpi(SM_CYCAPTION, window_dpi_);
			metrics[1] = GetSystemMetricsForDpi(SM_CXFIXEDFRAME, window_dpi_);
			metrics[2] = GetSystemMetricsForDpi(SM_CYSIZEFRAME, window_dpi_);
			metrics[3] = GetSystemMetricsForDpi(SM_CYBORDER, window_dpi_);

			const auto cy_titlebar_ = std::accumulate(metrics, metrics + sizeof metrics / sizeof(int), 0);
			MARGINS margins{ 0, 0, cy_titlebar_, 0 };
			DwmExtendFrameIntoClientArea(hwnd, &margins);
		}
		break;

		case WM_DESTROY: // IMPORTANT
			PostQuitMessage(0); // IMPORTANT
			break;

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				auto hdc = BeginPaint(hwnd, &ps);

        		//paint opaque:
				RECT rc{ 0, 0, 450, cy_titlebar_ };
				BP_PAINTPARAMS params = { sizeof(params), BPPF_NOCLIP | BPPF_ERASE };
				HDC memdc;
				HPAINTBUFFER hbuffer = BeginBufferedPaint(
					hdc, &rc, BPBF_TOPDOWNDIB, &params, &memdc);

				auto brush = CreateSolidBrush(RGB(70, 70, 70));
				FillRect(memdc, &rc, brush);
				DeleteObject(brush);

				SetBkMode(memdc, TRANSPARENT);
				DrawText(memdc, L"Anime Connect", -1, &rc, 0);
				BufferedPaintSetAlpha(hbuffer, &rc, 255);
				EndBufferedPaint(hbuffer, TRUE);

				EndPaint(hwnd, &ps);
				return 0;
			}

			case WM_NCCALCSIZE:
			{
				if(wParam)
				{
					RECT& r = reinterpret_cast<LPNCCALCSIZE_PARAMS>(lParam)->rgrc[0];
					r.left += border_thickness.left;
					r.right -= border_thickness.right;
					r.bottom -= border_thickness.bottom;
					return 0;
				}
				break;
			}

			case WM_NCHITTEST:
			{
				result = DefWindowProc(hwnd, uMsg, wParam, lParam);
				if(result == HTCLIENT)
				{
					POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
					ScreenToClient(hwnd, &pt);
					if(pt.y < border_thickness.top) return HTTOP;
					if(pt.y < cy_titlebar_)  return HTCAPTION;
				}
				return result;
			}

			default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam); // IMPORTANT
		}

		return 0;
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
