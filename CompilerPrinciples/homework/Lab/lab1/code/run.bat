@echo off
REM Compile all Java files in the script directory (ASCII-only messages)
setlocal enabledelayedexpansion

REM script directory (ends with backslash)
set "SCRIPT_DIR=%~dp0"

REM check javac
where javac >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo javac not found. Please install JDK and add javac to PATH.
    pause
    exit /b 1
)

echo Compiling Java files in %SCRIPT_DIR%...
pushd "%SCRIPT_DIR%" >nul 2>nul || (
    echo Cannot change to script directory %SCRIPT_DIR%
    pause
    exit /b 1
)

if not exist *.java (
    echo No Java files found in %SCRIPT_DIR%.
    pause
    popd >nul 2>nul
    exit /b 0
)

REM build list of source files excluding *Test.java
set "SRC_LIST="
for /f "delims=" %%F in ('dir /b *.java ^| findstr /i /v "Test.java"') do (
    set "SRC_LIST=!SRC_LIST! %%F"
)

if "%SRC_LIST%"=="" (
    echo No non-test Java files found in %SCRIPT_DIR%.
    pause
    popd >nul 2>nul
    exit /b 0
)

javac -encoding UTF-8 %SRC_LIST% -d .
set "RC=%ERRORLEVEL%"
popd >nul 2>nul

if %RC% NEQ 0 (
    echo Compilation failed. javac returned %RC%.
    pause
    exit /b %RC%
)

echo Compilation succeeded.
echo Running application PersonalTaxApp...
pushd "%SCRIPT_DIR%" >nul 2>nul || (
    echo Cannot change to script directory %SCRIPT_DIR% to run application.
    pause
    exit /b 1
)
java -cp . PersonalTaxApp %*
set "APP_RC=%ERRORLEVEL%"
popd >nul 2>nul
echo Application exited with code %APP_RC%
pause
endlocal