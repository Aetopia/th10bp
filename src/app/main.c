#include <shlwapi.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>

PVOID CDECL __wrap_memset(PVOID dst, BYTE data, SIZE_T count)
{
    __stosb(dst, data, count);
    return dst;
}

VOID CDECL WinMainCRTStartup()
{
    WCHAR mod[MAX_PATH] = {};
    WCHAR exe[MAX_PATH] = {};
    WCHAR dll[MAX_PATH] = {};

    GetModuleFileNameW(NULL, mod, MAX_PATH);
    PathRenameExtensionW(lstrcpyW(exe, mod), L".exe");
    PathRenameExtensionW(lstrcpyW(dll, mod), L".dll");

    if (CompareStringOrdinal(mod, -1, exe, -1, MAX_PATH) == CSTR_EQUAL)
        ExitProcess(EXIT_FAILURE);

    PROCESS_INFORMATION pi = {};
    CreateProcessW(exe, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &(STARTUPINFOW){}, &pi);

    PVOID param = VirtualAllocEx(pi.hProcess, NULL, sizeof dll, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    WriteProcessMemory(pi.hProcess, param, dll, sizeof dll, NULL);

    HANDLE thread = CreateRemoteThread(pi.hProcess, NULL, 0, (PVOID)LoadLibraryW, param, 0, NULL);
    WaitForSingleObject(thread, INFINITE);

    VirtualFreeEx(pi.hProcess, param, 0, MEM_RELEASE);
    CloseHandle(thread);

    ResumeThread(pi.hThread);
    CloseHandle(pi.hThread);

    CloseHandle(pi.hProcess);
    ExitProcess(EXIT_SUCCESS);
}