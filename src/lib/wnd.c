#pragma once
#include <windef.h>
#include <wingdi.h>
#include <winuser.h>
#include <winbase.h>

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

        INT cx = rc.right;
        INT cy = MulDiv(cx, 3, 4);

        if (cy > rc.bottom)
        {
            cy = rc.bottom;
            cx = MulDiv(cy, 4, 3);
        }

        INT x = (rc.right - cx) / 2;
        INT y = (rc.bottom - cy) / 2;

        SetWindowPos(g_hWnd, NULL, x, y, cx, cy, SWP_NOZORDER);
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

        SetWindowPos(hWnd, NULL, x, y, cx, cy, SWP_NOZORDER);
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