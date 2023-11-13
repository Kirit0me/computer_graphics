@echo off
gcc main.c image.c -o build.exe
if %errorlevel% neq 0 goto compile_error
echo Compilation successful!
build.exe
goto end

:compile_error
echo Compilation failed!

:end
