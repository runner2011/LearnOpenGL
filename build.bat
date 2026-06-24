@echo off
setlocal

set "BUILD_DIR=%~dp0build"
set "CONFIG=Debug"
set "TARGET=LearnOpenGL"

:parse
if "%~1"=="" goto build
if /I "%~1"=="Release" set "CONFIG=Release"
if /I "%~1"=="Debug" set "CONFIG=Debug"
if /I "%~1"=="clean" set "TARGET=clean"
if /I "%~1"=="run" set "RUN_AFTER_BUILD=1"
shift
goto parse

:build
cmake -S "%~dp0" -B "%BUILD_DIR%" || exit /b %ERRORLEVEL%
cmake --build "%BUILD_DIR%" --config "%CONFIG%" --target "%TARGET%" || exit /b %ERRORLEVEL%

if "%RUN_AFTER_BUILD%"=="1" if /I not "%TARGET%"=="clean" (
    if exist "%BUILD_DIR%\bin\%CONFIG%\LearnOpenGL.exe" (
        pushd "%BUILD_DIR%\bin\%CONFIG%"
        LearnOpenGL.exe
        popd
    ) else (
        pushd "%BUILD_DIR%\bin"
        LearnOpenGL.exe
        popd
    )
)

exit /b %ERRORLEVEL%
