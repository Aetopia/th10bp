#pragma once
#include <windef.h>
#include <wingdi.h>
#include <winuser.h>

HWND g_hWnd = {};
WNDPROC g_WndProc = {};

LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT: {
        PAINTSTRUCT paint = {};
        BeginPaint(hWnd, &paint);

        HBRUSH hbr = GetStockObject(BLACK_BRUSH);
        FillRect(paint.hdc, &paint.rcPaint, hbr);

        EndPaint(hWnd, &paint);
        break;
    }
    case WM_WINDOWPOSCHANGED: {
        RECT rc = {};
        GetClientRect(hWnd, &rc);

        FLOAT cx = rc.right;
        FLOAT cy = rc.bottom;

        FLOAT parent = cx / cy;
        FLOAT child = 4.F / 3.F;

        if (parent > child)
            cx = cy * child + .5F;
        else if (parent < child)
            cy = cx / child + .5F;

        FLOAT x = (rc.right - cx) / 2;
        FLOAT y = (rc.bottom - cy) / 2;

        SetWindowPos(g_hWnd, HWND_TOP, x, y, cx, cy, SWP_NOZORDER);
        break;
    }
    }
    return CallWindowProcA(g_WndProc, hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI FullScreenWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DISPLAYCHANGE:
    case WM_WINDOWPOSCHANGED: {
        MONITORINFO mi = {.cbSize = sizeof(MONITORINFO)};
        GetMonitorInfoA(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi);

        INT x = mi.rcMonitor.left;
        INT y = mi.rcMonitor.top;

        INT cx = mi.rcMonitor.right - x;
        INT cy = mi.rcMonitor.bottom - y;

        SetWindowPos(hWnd, HWND_TOP, x, y, cx, cy, SWP_NOZORDER);
    }
    }
    return WndProc(hWnd, uMsg, wParam, lParam);
}

LRESULT WINAPI WindowedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SETCURSOR:
        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    }
    return WndProc(hWnd, uMsg, wParam, lParam);
}