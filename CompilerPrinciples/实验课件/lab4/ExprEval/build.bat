@echo off
cd src
javac -d ..\bin -classpath ..\bin parser\*.java
cd ..
pause
@echo on
