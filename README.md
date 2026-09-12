# Touhou 10 Borderless Patch (th10bp)

Adds resizable window & borderless fullscreen support to Touhou 10 with aspect ratio preservation.

## Showcase

<div align="center">

[![](https://img.youtube.com/vi/19nlS8QVjpo/maxresdefault.jpg)](https://youtu.be/19nlS8QVjpo)

</div>

### Tweaks

- The game has Desktop Window Manager opt in for [MMCSS scheduling](https://learn.microsoft.com/windows/win32/api/dwmapi/nf-dwmapi-dwmenablemmcss).
- The game will no longer [block the Windows key](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417921(v=vs.85)) when DirectInput is being used.
- The game now uses a higher timer resolution which provides [better V-Sync quality](https://learn.microsoft.com/windows/win32/direct3d9/d3dpresent#remarks).

## Usage

- [Download](https://github.com/Aetopia/th10bp/releases/latest) the latest release of `th10bp`.
- Locate a copy of Touhou 10 on your system.
- Place `dinput8.dll` in the game's install directory.

## Build
1. Install [CMake](https://cmake.org) & [MinGW (x86)](https://www.mingw-w64.org).
2. Run [`BUILD.cmd`](BUILD.cmd) to build the project.