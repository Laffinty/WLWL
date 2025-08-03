@echo off
echo --- WLWL Interpreter Build Script ---

set COMPILER=gcc
set SOURCES=src/main.c src/core/logger.c src/core/dyn_array.c src/lexer/token.c src/lexer/lexer.c src/parser/ast.c src/parser/parser.c src/runtime/object.c src/runtime/environment.c src/runtime/builtins.c
set OUTPUT=build/wl.exe
set FLAGS=-o

rem Add the -Isrc flag to tell GCC where to find the header files.
set CFLAGS=-Isrc

echo Compiler: %COMPILER%
echo Sources:      %SOURCES%
rem Note: src/runtime/evaluator.c has been removed to avoid conflicts.
echo Output:   %OUTPUT%

echo Compiling and linking...
rem We add the %CFLAGS% to the command line.
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
