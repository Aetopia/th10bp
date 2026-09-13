#pragma once
#include "wnd.c"
#include <d3d9.h>
#include <MinHook.h>
#include <stdatomic.h>

HRESULT WINAPI (*g_Present)(PVOID, PVOID, PVOID, HWND, PVOID) = {};
HRESULT WINAPI (*g_CreateDevice)(PVOID, UINT, D3DDEVTYPE, HWND, DWORD, PVOID, PVOID) = {};

HRESULT WINAPI Reset(PVOID this, PVOID params)
{
    return D3D_OK;
}

HRESULT WINAPI Present(PVOID this, PVOID src, PVOID dst, HWND wnd, PVOID rgn)
{
    return g_Present(this, NULL, NULL, g_Wnd, NULL);
}

HRESULT WINAPI CreateDevice(PVOID this, UINT adapter, D3DDEVTYPE type, HWND wnd, DWORD flags,
                            D3DPRESENT_PARAMETERS *params, LPDIRECT3DDEVICE9 *device)
{
    static atomic_flag s_flag = {};

    D3DPRESENT_PARAMETERS d3dpp = *params;
    DWORD style = d3dpp.Windowed ? WS_OVERLAPPEDWINDOW : WS_POPUP;
    WNDPROC procedure = d3dpp.Windowed ? WindowedWndProc : FullScreenWndProc;

    if (IsWindowVisible(wnd))
        style |= WS_VISIBLE;

    d3dpp.Windowed = TRUE;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    flags |= D3DCREATE_NOWINDOWCHANGES;
    HRESULT hr = g_CreateDevice(this, adapter, type, wnd, flags, &d3dpp, device);

    if (SUCCEEDED(hr) && !atomic_flag_test_and_set(&s_flag))
    {
        MH_CreateHook((*device)->lpVtbl->Reset, Reset, NULL);
        MH_CreateHook((*device)->lpVtbl->Present, Present, (PVOID)&g_Present);

        MH_QueueEnableHook((*device)->lpVtbl->Reset);
        MH_QueueEnableHook((*device)->lpVtbl->Present);
        MH_ApplyQueued();

        PSTR atom = MAKEINTATOM(RegisterClassA(&(WNDCLASSA){
            .lpszClassName = "PRESENT",
            .lpfnWndProc = DefWindowProcA,
            .hCursor = LoadCursorA(NULL, IDC_ARROW),
            .hbrBackground = GetStockObject(BLACK_BRUSH),
        }));

        g_Wnd = CreateWindowExA(WS_EX_LEFT, atom, NULL, WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, wnd, NULL, NULL, NULL);
        g_WndProc = (PVOID)SetWindowLongA(wnd, GWLP_WNDPROC, (LONG_PTR)procedure);

        SetWindowLongA(wnd, GWL_STYLE, style);
        SetWindowLongA(wnd, GWL_EXSTYLE, WS_EX_APPWINDOW);

        SetWindowPos(wnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    return hr;
}