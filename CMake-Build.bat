@echo off

mkdir build > nul
cd build > nul
echo [+] Preparing files...
cmake .. > nul
echo [+] Compiling...
cmake --build . > nul
cd .. > nul
move "build\Debug\SnapKey.exe" "." > nul
echo [+] Done!
rmdir /s /q build > nul
pause > nul