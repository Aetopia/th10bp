@echo off

cd "%~dp0"
cd "src"

rd /q /s "bin"
rd /q /s "obj"

md "bin"
md "obj"

windres.exe -i "lib\res.rc" -o "obj\lib.o"
windres.exe -i "app\res.rc" -o "obj\app.o"

cc.exe -O3 -s -Wl,--gc-sections,--exclude-all-symbols,--wrap=memset -nostdlib -mwindows -DWIN32_LEAN_AND_MEAN "app\main.c" "obj\app.o" -lkernel32 -lshlwapi -o "bin\th10.com"
cc.exe -O3 -s -Wl,--gc-sections,--exclude-all-symbols,--wrap=memcpy,--wrap=memset -static -shared -nostdlib -e DllMain -DINITGUID -DWIN32_LEAN_AND_MEAN -DWINVER=NTDDI_WIN10 "lib\main.c" "obj\lib.o" -lminhook -lkernel32 -luser32 -ld3d9 -ldinput8 -lgdi32 -ldwmapi -o "bin\th10.dll"

powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -Command "$ProgressPreference = 'SilentlyContinue'; Compress-Archive -Path 'bin\*' -DestinationPath 'bin\th10bp.zip' -Force"