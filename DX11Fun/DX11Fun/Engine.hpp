//
// DX11 Fun.
// Engine.
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Engine {
public:
    Engine();
    ~Engine();

    void Startup();
    void Shutdown();
    void Run();

    LRESULT CALLBACK _WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    LPCWSTR m_lpszApplicationName;

    void InitWindow();
    void CleanupWindow();
};

// Proxy for our WndProc in the class.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Used to call the internal WndProc... yeah yeah, not super safe.
static Engine* hEngine;