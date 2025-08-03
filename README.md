@echo off
echo --- WLWL Interpreter Build Script (Fixed) ---

set COMPILER=gcc
set SOURCES=src/main.c src/core/logger.c src/core/dyn_array.c src/lexer/token.c src/lexer/lexer.c src/parser/ast.c src/parser/parser.c src/runtime/object.c src/runtime/environment.c src/runtime/builtins.c
set OUTPUT=build/wl.exe
set FLAGS=-o
set CFLAGS=-Isrc -Wall -Wextra

if not exist build mkdir build

echo Compiler: %COMPILER%
echo Sources: %SOURCES%
echo Output: %OUTPUT%
echo Flags: %CFLAGS%

echo Compiling and linking...
%COMPILER% %CFLAGS% %FLAGS% %OUTPUT% %SOURCES%

if %errorlevel% neq 0 (
    echo.
    echo ********************
    echo * BUILD FAILED!    *
    echo ********************
    exit /b 1
)

echo.
echo ********************
echo * BUILD SUCCEEDED! *
echo ********************
echo.
echo You can now run: build/wl.exe