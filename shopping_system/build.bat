@echo off
rem ------------------------------------------------------------
rem Build script for Shopping System (Windows, MinGW-w64)
rem ------------------------------------------------------------
setlocal EnableDelayedExpansion

rem Ensure GCC is in PATH - prioritize WinLibs 64-bit if available
if exist "C:\Users\AMEYA\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\g++.exe" (
  set "PATH=C:\Users\AMEYA\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin;!PATH!"
)
if exist "C:\gcc14\mingw64\bin\g++.exe" (
  set "PATH=C:\gcc14\mingw64\bin;!PATH!"
)

rem Compiler and flags (static linking prevents 32-bit/64-bit DLL collisions on Windows)
set CXX=g++
set CXXFLAGS=-std=c++17 -Wall -O2 -pthread -static -static-libgcc -static-libstdc++

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
