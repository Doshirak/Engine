#include"engine.h"

Engine::Engine(HINSTANCE hInstance, int nCmdShow){
	window.init(hInstance, nCmdShow);
	device.g_hInst = window.g_hInst;
	device.g_hWnd = window.g_hWnd;
	device.init();
}

Engine::~Engine(){
}

int Engine::run() {
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			device.render();
		}
	}

	device.cleanup();
	return  (int)msg.wParam;
}

