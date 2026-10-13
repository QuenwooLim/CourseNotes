@echo off
cd src
javadoc -private -author -version -d ..\doc -classpath ..\bin parser\*.java
cd ..
pause
@echo on
