@echo off
cls

REM set vars
set C_COMPILER=gcc.exe
set CPP_COMPILER=g++.exe
set RC_COMPILER=windres.exe
set MAKE_BIN=mingw32-make.exe

REM build
cd build/make
cmake ../.. -G"MinGW Makefiles" -DCMAKE_RC_COMPILER=%RC_COMPILER% -DCMAKE_C_COMPILER=%C_COMPILER% -DCMAKE_CXX_COMPILER=%CPP_COMPILER% -DCMAKE_MAKE_PROGRAM=%MAKE_BIN% -DCMAKE_C_COMPILER_WORKS=1 -DCMAKE_CXX_COMPILER_WORKS=1 || goto error
mingw32-make.exe || goto error

REM execute
cd ../../
start build/make/SAHEngine.exe
goto end

:error
cd ../../

:end
