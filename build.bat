@echo off
setlocal enableextensions enabledelayedexpansion

rem // ------------------------------------------------------------------------
rem // Some boilerplate to set up the VC++ build environment.
rem // ------------------------------------------------------------------------
path %ProgramFiles(x86)%\Microsoft Visual Studio\Installer;%PATH%
set VCVARS64_BAT=""
for /f "usebackq tokens=*" %%i in (`vswhere -version 17 -find "**\vcvars64.bat"`) do (
    set VCVARS64_BAT="%%i"
)
if %VCVARS64_BAT%=="" (
    echo Visual Studio 2022 installation not found.
    exit /B 1
)
call %VCVARS64_BAT%

rem // ------------------------------------------------------------------------

pushd "%~dp0"

set BIN_PATH=bin
if not exist "%BIN_PATH%" (
    mkdir "%BIN_PATH%"
)

call :build_proxy_dll d3d11.dll
call :build_proxy_dll dxgi.dll
call :build_proxy_dll dinput8.dll
call :build_dll proxydll.h.template SekiroFpsUnlockDll.dll

exit /B

rem // ------------------------------------------------------------------------

:build_proxy_dll
call :build_dll proxydll\%1.h %1
exit /B

:build_dll
copy src\%1 src\proxydll.h
@rem Update the last modified date of the file so that msbuild knows to rebuild.
copy /b src\proxydll.h+,, src\proxydll.h
msbuild SekiroFpsUnlockDll.sln -m -p:Configuration=Release
copy x64\Release\SekiroFpsUnlockDll.dll %BIN_PATH%\%2
exit /B 0