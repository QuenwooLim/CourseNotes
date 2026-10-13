@echo off
REM Compile Oberon-0 lexical analyzer
javac -cp bin -d bin src\exceptions\*.java src\Token.java src\OberonScanner.java src\ScannerTest.java
echo Build complete.
