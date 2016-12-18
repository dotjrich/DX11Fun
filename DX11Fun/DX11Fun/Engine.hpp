//
// DX11 Fun.
// Engine.
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dxgi.h>
#include <d3d11.h>

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

    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pDeviceContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11RenderTargetView* m_pBackBuffer;

    void InitWindow();
    void CleanupWindow();

    void InitD3D();
    void CleanupD3D();
};

// Proxy for our WndProc in the class.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Used to call the internal WndProc... yeah yeah, not super safe.
static Engine* hEngine;