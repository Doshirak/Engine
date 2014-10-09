#include <windows.h>
#include "engine.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ) {
	Engine engine(hInstance, nCmdShow);
	return engine.run();
}