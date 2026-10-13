@echo off
cd src
javac -d ..\bin -classpath ..\lib\jgraph.jar;..\lib\callgraph.jar;..\lib\flowchart.jar *.java
cd ..
pause
@echo on
