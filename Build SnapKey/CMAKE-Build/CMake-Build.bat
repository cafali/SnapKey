@echo off
where cmake.exe >nul || (
  echo [-] CMake doesn't exist. Please, download it.
  echo [~] Press any key to exit...
  pause > nul
  exit /b 1
)
mkdir build > nul
cd build > nul
echo [+] Preparing files...
cmake .. > nul
echo [+] Compiling...
cmake --build . > nul
cd .. > nul
if not exist "build\Debug\SnapKey.exe" (
  echo [-] Error occured while building!
  echo [~] Press any key to exit...
  pause > nul
  rmdir /s /q build > nul
  exit /b 1
)
move "build\Debug\SnapKey.exe" "." > nul
echo [+] Done!
rmdir /s /q build > nul
echo [~] Press any key to exit...
pause > nul
exit /b 0