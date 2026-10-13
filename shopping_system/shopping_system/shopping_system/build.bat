@echo off
rem ------------------------------------------------------------
rem Build script for Shopping System (Windows, MinGW-w64)
rem ------------------------------------------------------------
setlocal EnableDelayedExpansion

rem Ensure GCC is in PATH – adjust if necessary
rem set "PATH=C:\gcc14\mingw64\bin;%PATH%"

rem Compiler and flags
set CXX=g++
set CXXFLAGS=-std=c++17 -Wall -O2 -pthread

rem Build server executable
echo Building shop_server.exe ...
%CXX% %CXXFLAGS% server.cpp -o shop_server.exe -lws2_32 -lwsock32
if errorlevel 1 (
  echo [ERROR] Failed to build shop_server.exe
  exit /b 1
)

rem Build console executable
echo Building shop_console.exe ...
%CXX% %CXXFLAGS% console\console_main.cpp -o shop_console.exe
if errorlevel 1 (
  echo [ERROR] Failed to build shop_console.exe
  exit /b 1
)

echo Build completed successfully.
exit /b 0
