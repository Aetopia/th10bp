#pragma once
#include <dinput.h>
#include <dinputd.h>

HRESULT WINAPI (*g_SetCooperativeLevel)(PVOID, HWND, DWORD) = {};
HRESULT WINAPI (*g_DirectInput8Create)(PVOID, DWORD, REFIID, PVOID, PVOID) = {};

HRESULT WINAPI SetCooperativeLevel(PVOID this, HWND wnd, DWORD flags)
{
    flags &= ~DISCL_NOWINKEY;
    return g_SetCooperativeLevel(this, wnd, flags);
}

__declspec(dllexport) HRESULT WINAPI DirectInput8Create(HINSTANCE instance, DWORD version, REFIID iid, LPVOID *dinput,
                                                        LPUNKNOWN unknown)
{
    return g_DirectInput8Create(instance, version, iid, dinput, unknown);
}