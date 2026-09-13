#include "d3d9.c"
#include <dwmapi.h>
#include <shlwapi.h>

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

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, PVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(instance);
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        MH_Initialize();

        LPDIRECT3D9 d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
        MH_CreateHook(d3d9->lpVtbl->CreateDevice, CreateDevice, (PVOID)&g_CreateDevice);

        MH_EnableHook(d3d9->lpVtbl->CreateDevice);
        d3d9->lpVtbl->Release(d3d9);

        DwmEnableMMCSS(TRUE);
    }
    return TRUE;
}