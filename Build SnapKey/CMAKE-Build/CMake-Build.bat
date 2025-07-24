@echo off
setlocal

rem ========== Â·¾¶ÅäÖÃ ==========
set THISDIR=%~dp0
set CMAKE=%THISDIR%cmake-4.1.0-rc2-windows-x86_64\bin\cmake.exe
set MINGW=%THISDIR%mingw64
set PATH=%MINGW%\bin;%PATH%

rem ========== ±àÒë¹ý³Ì ==========
echo [+] Configuring CMake...
"%CMAKE%" -S "%THISDIR%..\.." -B "%THISDIR%build" -G "MinGW Makefiles"

if %errorlevel% neq 0 (
    echo [!] CMake configure failed.
    pause
    exit /b 1
)

echo [+] Building with MinGW...
"%CMAKE%" --build "%THISDIR%build"

if exist "%THISDIR%build\SnapKey.exe" (
    copy /Y "%THISDIR%build\SnapKey.exe" "%THISDIR%" >nul
)

echo [+] Done!
pause
