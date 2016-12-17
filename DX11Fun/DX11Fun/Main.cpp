//
// DX11 Fun.
// Entry point.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Engine.hpp"

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Engine engine;

    engine.Startup();
    engine.Run();
    engine.Shutdown();

    return 0;
}