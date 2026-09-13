#include <dinput.h>
#include <dinputd.h>
#include <shlwapi.h>
#include <MinHook.h>
#include <stdatomic.h>

HRESULT WINAPI (*g_SetCooperativeLevel)(PVOID, HWND, DWORD) = {};

HRESULT WINAPI SetCooperativeLevel(PVOID this, HWND wnd, DWORD flags)
{
    flags &= ~DISCL_NOWINKEY;
    return g_SetCooperativeLevel(this, wnd, flags);
}

__declspec(dllexport) HRESULT WINAPI DirectInput8Create(HINSTANCE instance, DWORD version, REFIID iid, LPVOID *object,
                                                        LPUNKNOWN unknown)
{
    static atomic_flag s_flag = {};
    static HRESULT WINAPI (*s_DirectInput8Create)(PVOID, DWORD, REFIID, PVOID, PVOID) = {};

    if (!s_DirectInput8Create)
    {
        WCHAR path[MAX_PATH] = {};

        GetSystemDirectoryW(path, MAX_PATH);
        PathCombineW(path, path, L"dinput8.dll");

        HMODULE module = LoadLibraryW(path);
        s_DirectInput8Create = (PVOID)GetProcAddress(module, "DirectInput8Create");
    }

    HRESULT hr = s_DirectInput8Create(instance, version, iid, object, unknown);

    if (SUCCEEDED(hr) && !atomic_flag_test_and_set(&s_flag))
    {
        PVOID target = {};

        if (InlineIsEqualGUID(iid, &IID_IDirectInput8A))
        {
            LPDIRECTINPUT8A dinput = *object;
            LPDIRECTINPUTDEVICE8A device = {};

            dinput->lpVtbl->CreateDevice(dinput, &GUID_SysMouseEm, &device, NULL);
            target = device->lpVtbl->SetCooperativeLevel;
           
            device->lpVtbl->Release(device);
        }

        else if (InlineIsEqualGUID(iid, &IID_IDirectInput8W))
        {
            LPDIRECTINPUT8W dinput = *object;
            LPDIRECTINPUTDEVICE8W device = {};

            dinput->lpVtbl->CreateDevice(dinput, &GUID_SysMouseEm, &device, NULL);
            target = device->lpVtbl->SetCooperativeLevel;
           
            device->lpVtbl->Release(device);
        }

        MH_CreateHook(target, SetCooperativeLevel, (PVOID)&g_SetCooperativeLevel);
        MH_EnableHook(target);
    }

    return hr;
}