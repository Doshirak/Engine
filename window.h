#pragma once
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//HINSTANCE               g_hInst = NULL;
//HWND                    g_hWnd = NULL;

class Window {
public:
	Window();
	~Window();
	HRESULT init(HINSTANCE hInstance, int nCmdShow);
	HINSTANCE               g_hInst = NULL;
	HWND                    g_hWnd = NULL;
private:

};
