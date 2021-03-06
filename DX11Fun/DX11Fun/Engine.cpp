//
// DX11 Fun.
// Engine.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dxgi.h>
#include <d3d11.h>

#include "Engine.hpp"

#define _WIDTH 800
#define _HEIGHT 600

// -----------------------------------------------------------------------
// Public.
// -----------------------------------------------------------------------

Engine::Engine()
    : m_hInstance(GetModuleHandle(NULL)), m_hWnd(NULL), m_lpszApplicationName(L"DX11Fun")
{
    hEngine = NULL;
}

// -----------------------------------------------------------------------

Engine::~Engine()
{
}

// -----------------------------------------------------------------------

void
Engine::Startup()
{
    InitWindow();
    InitD3D();

    hEngine = this;
}

// -----------------------------------------------------------------------

void
Engine::Shutdown()
{
    CleanupD3D();
    CleanupWindow();

    hEngine = NULL;
}

// -----------------------------------------------------------------------

void
Engine::Run()
{
    if (hEngine == NULL) {
        MessageBox(NULL, L"Engine not properly initialized.", NULL, MB_ICONERROR | MB_OK);
        return;
    }

    bool run = true;
    MSG msg;

    while (run) {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                run = false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            RenderFrame();
        }
    }
}

// -----------------------------------------------------------------------

LRESULT CALLBACK
Engine::_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT ret;

    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        ret = 0;

    default:
        ret = DefWindowProc(hWnd, message, wParam, lParam);
    }

    return ret;
}

// -----------------------------------------------------------------------
// Private.
// -----------------------------------------------------------------------

void
Engine::InitWindow()
{
    WNDCLASSEX wcex;
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = m_hInstance;
    wcex.hIcon         = LoadIcon(m_hInstance, IDI_APPLICATION);
    wcex.hIconSm       = LoadIcon(m_hInstance, IDI_APPLICATION);
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = m_lpszApplicationName;

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, L"Failed to register window class.", NULL, MB_ICONERROR | MB_OK);
        return;
    }

    // Set up our client area based on our width and height.
    RECT wr = { 0, 0, _WIDTH, _HEIGHT };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

    m_hWnd = CreateWindow(m_lpszApplicationName, L"DX11 Fun", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, m_hInstance, NULL);
    if (!m_hWnd) {
        MessageBox(NULL, L"Failed to create window.", NULL, MB_ICONERROR | MB_OK);
        return;
    }

    ShowWindow(m_hWnd, SW_SHOW);
}

// -----------------------------------------------------------------------

void
Engine::CleanupWindow()
{
    if (m_hWnd != NULL) {
        DestroyWindow(m_hWnd);
        m_hWnd = NULL;

        UnregisterClass(m_lpszApplicationName, m_hInstance);
    }
}

// -----------------------------------------------------------------------

void
Engine::InitD3D()
{
    // Initialize our device, device context, and swap chain.
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = m_hWnd;
    scd.SampleDesc.Count = 4;
    scd.Windowed = true;

    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext);

    // Set up the back buffer.
    ID3D11Texture2D* pBackBuffer;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    
    m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pBackBuffer);
    pBackBuffer->Release();

    m_pDeviceContext->OMSetRenderTargets(1, &m_pBackBuffer, NULL);

    // Set up viewport.
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = _WIDTH;
    viewport.Height = _HEIGHT;

    m_pDeviceContext->RSSetViewports(1, &viewport);
}

// -----------------------------------------------------------------------

void
Engine::CleanupD3D()
{
    m_pBackBuffer->Release();
    m_pSwapChain->Release();
    m_pDevice->Release();
    m_pDeviceContext->Release();
}

// -----------------------------------------------------------------------

void
Engine::RenderFrame()
{
    static float rgClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer, rgClearColor);

    m_pSwapChain->Present(0, 0);
}

// -----------------------------------------------------------------------
// External to class.
// -----------------------------------------------------------------------

LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return hEngine->_WndProc(hWnd, message, wParam, lParam);
}