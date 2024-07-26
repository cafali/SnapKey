
mkdir build
cd build
cmake ..
cmake --build .
cd ..
move "build\Debug\SnapKey.exe" "."
rmdir /s /q build