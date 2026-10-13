@echo off
REM Windows build script - mirrors build.sh behavior for Windows (cmd)
setlocal EnableDelayedExpansion

if not exist bin mkdir bin
if not exist doc mkdir doc
if not exist lib mkdir lib

set "SOURCES="
for /R %%F in (*.java) do (
    set "SOURCES=!SOURCES! "%%F""
)

if "%SOURCES%"=="" (
    echo No Java sources found under src
    exit /b 1
)

javac -d bin %SOURCES%
if errorlevel 1 (
    echo Compilation failed.
    exit /b 1
)

echo Compile completed. Classes are in bin/

REM generate javadoc into doc/
where javadoc >nul 2>&1
if %ERRORLEVEL%==0 (
    javadoc -d doc -encoding UTF-8 -charset UTF-8 %SOURCES% || echo Javadoc generation failed
    echo Javadoc generated in doc/
) else (
    echo javadoc not found; skipping doc generation
)

REM JUnit test support: expect user to provide console-standalone jar in lib\
set "JUNIT_JAR=lib\junit-platform-console-standalone.jar"
set "JUNIT_URL=https://repo1.maven.org/maven2/org/junit/platform/junit-platform-console-standalone/1.9.4/junit-platform-console-standalone-1.9.4.jar"

set "TEST_SOURCES="
for /R src %%F in (*Test.java) do set "TEST_SOURCES=!TEST_SOURCES! "%%F""
for /R test %%F in (*Test.java) do set "TEST_SOURCES=!TEST_SOURCES! "%%F""

if defined TEST_SOURCES (
    echo Found test sources. Ensure JUnit jar is present at %JUNIT_JAR% to run tests.
    if not exist "%JUNIT_JAR%" (
        echo Skipping tests: %JUNIT_JAR% not found.
        echo Please download %JUNIT_URL% and save it as %JUNIT_JAR%
    ) else (
        echo Compiling tests...
        javac -cp "bin;%JUNIT_JAR%" -d bin %TEST_SOURCES%
        if errorlevel 1 (
            echo Test compilation failed.
            exit /b 1
        )
        echo Running tests...
        java -jar "%JUNIT_JAR%" execute --scan-classpath --class-path bin
    )
) else (
    echo No test sources found (src/*Test.java or test/*Test.java)
)

echo Build completed.
endlocal
