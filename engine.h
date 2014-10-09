#pragma once
#include <windows.h>
#include "window.h"
#include "device.h"

class Engine {
public:
	Engine(HINSTANCE hInstance, int nCmdShow);
	~Engine();
	int run();
private:
	Window window;
	Device device;
};