@echo off
echo --- WLWL Interpreter Build Script ---

set COMPILER=gcc
set SOURCES=src/main.c src/core/logger.c src/core/dyn_array.c src/lexer/token.c src/lexer/lexer.c src/parser/ast.c src/parser/parser.c src/runtime/object.c src/runtime/environment.c src/runtime/builtins.c
set OUTPUT=build/wlwl.exe
set FLAGS=-o
set CFLAGS=-Isrc -Wall -Wextra

REM --- Handle Build Modes (Debug/Release) ---
set BUILD_MODE=Release
set DEBUG_FLAG=
if /I "%1" == "debug" (
    set BUILD_MODE=Debug
    set DEBUG_FLAG=-DDEBUG_MODE
)

if not exist build mkdir build

echo.
echo Building WLWL Interpreter (%BUILD_MODE% mode)...
echo -------------------------------------------------
echo Compiler: %COMPILER%
echo Output:   %OUTPUT%
echo Flags:    %CFLAGS% %DEBUG_FLAG%
echo -------------------------------------------------
echo.

echo Compiling and linking...
%COMPILER% %CFLAGS% %DEBUG_FLAG% %FLAGS% %OUTPUT% %SOURCES%

if %errorlevel% neq 0 (
    echo.
    echo ********************
    echo * BUILD FAILED!    *
    echo ********************
    exit /b 1
)

echo.
echo ***********************************
echo * BUILD SUCCEEDED! (%BUILD_MODE% mode) *
echo ***********************************
echo.
echo You can now run: %OUTPUT%
