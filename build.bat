@echo off
gcc main.c image.c image_processing.c -Wall -Wextra -Wno-unused-parameter -O3 -o build.exe

if %errorlevel% neq 0 goto compile_error
echo Compilation successful!

REM Run build.exe with command line arguments
build.exe %1

REM Check if build.exe was successful
if %errorlevel% neq 0 goto build_error

REM View the generated image using i_view64.exe
i_view64.exe cococola.ppm ;
REM ffmpeg -framerate 16 -i ".\julia_images\julia_image_%04d.ppm" -vf tblend, gblur -pix_fmt yuv420p -c:v libx265 out.mp4 ; 

goto end

:compile_error
echo Compilation failed!
goto end

:build_error
echo Error running build.exe!
goto end

:end
