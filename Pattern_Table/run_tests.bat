@echo off
REM ============================================================================
REM Build and Run Script for Pattern_Table Tests
REM Author: Tobi
REM Date:   04/04/2026
REM ============================================================================

setlocal enabledelayedexpansion

REM Check for compiler
set CC=
where gcc >nul 2>&1
if %errorlevel%==0 set CC=gcc

where clang >nul 2>&1
if %errorlevel%==0 set CC=clang

if "%CC%"=="" (
    echo ERROR: No C compiler found! Please install GCC or Clang.
    echo.
    echo For Windows, you can:
    echo   - Install MinGW: https://www.mingw-w64.org/
    echo   - Install MSYS2: https://www.msys2.org/
    echo   - Use WSL with apt install gcc
    echo.
    pause
    exit /b 1
)

echo ============================================
echo Pattern_Table Build Script
echo Using compiler: %CC%
echo ============================================
echo.

REM Create build directory
if not exist build mkdir build

REM ============================================================================
REM Build Original CMD_LINE Test
REM ============================================================================
echo [1/4] Building original cmd_line test...
%CC% -Wall -Wextra -std=c99 -g -I. -I../common ^
    -c ../common/cmd_line.c -o build\cmd_line.o 2>nul

if %errorlevel% neq 0 (
    echo ERROR: Failed to compile cmd_line.c
    pause
    exit /b 1
)

%CC% -Wall -Wextra -std=c99 -g -I. -I../common ^
    -c unit_test/cmd_line_test.c -o build\cmd_line_test.o 2>nul

if %errorlevel% neq 0 (
    echo ERROR: Failed to compile cmd_line_test.c
    pause
    exit /b 1
)

%CC% -Wall -Wextra -std=c99 -g -I. -I../common ^
    -c main_original.c -o build\main_original.o 2>nul

if %errorlevel% neq 0 (
    echo ERROR: Failed to compile main_original.c
    pause
    exit /b 1
)

%CC% -o build\test_cmd_line_original.exe ^
    build\cmd_line.o build\cmd_line_test.o build\main_original.o 2>nul

if %errorlevel% neq 0 (
    echo ERROR: Failed to link test_cmd_line_original
    pause
    exit /b 1
)

echo [OK] Original cmd_line test built successfully
echo.

REM ============================================================================
REM Build Custom CMD_LINE Test
REM ============================================================================
echo [2/4] Building custom cmd_line test...
%CC% -Wall -Wextra -std=c99 -g -I. -I../common ^
    -c my_cmd_line/my_cmd_line.c -o build\my_cmd_line.o 2>nul

if %errorlevel% neq 0 (
    echo ERROR: Failed to compile my_cmd_line.c
    pause
    exit /b 1
)

%CC% -Wall -Wextra -std=c99 -g -I. -I../common ^
    -c my_cmd_line/my_cmd_line_test.c -o build\my_cmd_line_test.o 2>nul

if %errorlevel% neq 0 (
    echo ERROR: Failed to compile my_cmd_line_test.c
    pause
    exit /b 1
)

%CC% -Wall -Wextra -std=c99 -g -I. -I../common ^
    -c main_my.c -o build\main_my.o 2>nul

if %errorlevel% neq 0 (
    echo ERROR: Failed to compile main_my.c
    pause
    exit /b 1
)

%CC% -o build\test_cmd_line_my.exe ^
    build\my_cmd_line.o build\my_cmd_line_test.o build\main_my.o 2>nul

if %errorlevel% neq 0 (
    echo ERROR: Failed to link test_cmd_line_my
    pause
    exit /b 1
)

echo [OK] Custom cmd_line test built successfully
echo.

REM ============================================================================
REM Run Tests
REM ============================================================================
echo [3/4] Running original cmd_line tests...
echo ============================================
build\test_cmd_line_original.exe
set RESULT1=%errorlevel%
echo.

echo [4/4] Running custom cmd_line tests...
echo ============================================
build\test_cmd_line_my.exe
set RESULT2=%errorlevel%
echo.

REM ============================================================================
REM Summary
REM ============================================================================
echo ============================================
echo Build and Test Summary
echo ============================================
if %RESULT1%==0 (
    echo [OK] Original cmd_line tests: PASSED
) else (
    echo [FAIL] Original cmd_line tests: FAILED
)

if %RESULT2%==0 (
    echo [OK] Custom cmd_line tests: PASSED
) else (
    echo [FAIL] Custom cmd_line tests: FAILED
)
echo ============================================
echo.

pause
exit /b 0
