@echo off
REM Run script for Windows
setlocal

if not exist bin (
  echo bin directory not found. Run build.bat first.
  exit /b 1
)

if "%1"=="-batch" (
  if "%2"=="" (
    echo Usage: run.bat -batch fileName
    exit /b 1
  )
  java -cp "bin;lib/*" agenda.AgendaService -batch "%2"
) else (
  java -cp "bin;lib/*" agenda.AgendaService
)

endlocal
