#include "d3d9.c"
#include <dwmapi.h>
#include <dinput.h>
#include <dinputd.h>

PVOID CDECL __wrap_memcpy(PVOID dst, PVOID src, SIZE_T count)
{
    __movsb(dst, src, count);
    return dst;
}

PVOID CDECL __wrap_memset(PVOID dst, BYTE data, SIZE_T count)
{
    __stosb(dst, data, count);
    return dst;
}

HRESULT WINAPI (*g_SetCooperativeLevel)(PVOID, HWND, DWORD) = {};

HRESULT WINAPI SetCooperativeLevel(PVOID this, HWND wnd, DWORD flags)
{
    flags &= ~DISCL_NOWINKEY;
    return g_SetCooperativeLevel(this, wnd, flags);
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, PVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(instance);
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        LPDIRECTINPUT8A dinput = {};
        LPDIRECTINPUTDEVICE8A device = {};
        LPDIRECT3D9 d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

        DirectInput8Create(instance, DIRECTINPUT_VERSION, &IID_IDirectInput8A, (PVOID)&dinput, NULL);
        dinput->lpVtbl->CreateDevice(dinput, &GUID_SysMouseEm, &device, NULL);

        MH_Initialize();
        MH_CreateHook(d3d9->lpVtbl->CreateDevice, CreateDevice, (PVOID)&g_CreateDevice);
        MH_CreateHook(device->lpVtbl->SetCooperativeLevel, SetCooperativeLevel, (PVOID)&g_SetCooperativeLevel);

        MH_QueueEnableHook(d3d9->lpVtbl->CreateDevice);
        MH_QueueEnableHook(device->lpVtbl->SetCooperativeLevel);
        MH_ApplyQueued();

        d3d9->lpVtbl->Release(d3d9);
        device->lpVtbl->Release(device);
        dinput->lpVtbl->Release(dinput);

        DwmEnableMMCSS(TRUE);
    }
    return TRUE;
}