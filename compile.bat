@echo off
cls

REM set vars
set C_COMPILER=gcc.exe
set CPP_COMPILER=g++.exe
set RC_COMPILER=windres.exe
set MAKE_BIN=mingw32-make.exe

REM create build directory
mkdir build
cd build

REM cmake project
echo ---------------- Compiling ----------------
cmake .. -G"MinGW Makefiles" -DCMAKE_RC_COMPILER=%RC_COMPILER% -DCMAKE_C_COMPILER=%C_COMPILER% -DCMAKE_CXX_COMPILER=%CPP_COMPILER% -DCMAKE_MAKE_PROGRAM=%MAKE_BIN% -DCMAKE_C_COMPILER_WORKS=1 -DCMAKE_CXX_COMPILER_WORKS=1 || goto error

REM make project
mingw32-make.exe || goto error
echo ------------ build successfull ------------

REM execute program
cd ..
start build/SAHEngine.exe
goto end

:error
cd ..

:end
