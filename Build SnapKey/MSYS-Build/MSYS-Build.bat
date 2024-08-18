
@echo off
setlocal

echo Compiling SnapKey...


REM Get the directory of the batch file
set "SCRIPT_DIR=%~dp0"

REM Set the path to your MSYS2 installation
set "MSYS2_PATH=C:\msys64\usr\bin\bash.exe"

REM Run the MSYS2 shell to compile the program
"%MSYS2_PATH%" -c "cd '%SCRIPT_DIR%' && ./MSYS-Build.sh"

REM Pause to keep the command window open
pause

endlocal
