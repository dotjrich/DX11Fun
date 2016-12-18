//
// DX11 Fun.
// Engine.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Engine.hpp"

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

    hEngine = this;
}

// -----------------------------------------------------------------------

void
Engine::Shutdown()
{
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

    // We want an 800x600 client area.
    RECT wr = { 0, 0, 800, 600 };
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

LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return hEngine->_WndProc(hWnd, message, wParam, lParam);
}