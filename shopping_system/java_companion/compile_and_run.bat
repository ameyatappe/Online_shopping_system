@echo off
setlocal EnableDelayedExpansion
echo ==============================================================
echo   ShopX Java OOP Companion - Compile and Run
echo ==============================================================

rem Check if bin directory exists, create if missing
if not exist "bin" mkdir "bin"

rem Try standard javac first
where javac >nul 2>nul
if !errorlevel! equ 0 (
    echo [INFO] Found javac in PATH. Compiling sources...
    javac -d bin src\*.java
    if !errorlevel! neq 0 (
        echo [ERROR] Compilation failed.
        exit /b 1
    )
) else (
    echo [INFO] javac not found in PATH. Using bundled portable compiler ecj.jar ...
    java -jar ecj.jar -8 -d bin src\*.java
    if !errorlevel! neq 0 (
        echo [ERROR] Portable compilation failed.
        exit /b 1
    )
)

echo [INFO] Compilation successful. Running ShopXDemo...
echo.
java -cp bin com.shopx.ShopXDemo
exit /b 0
