@echo off
REM test-junit.bat — Run JUnit tests using junit-platform-console-standalone.jar
setlocal

set "BIN_DIR=%~dp0bin"
set "JUNIT_JAR=%~dp0lib\junit-platform-console-standalone.jar"

if not exist "%JUNIT_JAR%" (
  echo ERROR: JUnit jar not found: "%JUNIT_JAR%"
  echo Please place junit-platform-console-standalone.jar into the lib folder.
  endlocal
  exit /b 1
)

echo Running JUnit tests...
java -jar "%JUNIT_JAR%" execute --class-path "%BIN_DIR%" --select-class PostfixTest
set "RC=%ERRORLEVEL%"

endlocal
exit /b %RC%
