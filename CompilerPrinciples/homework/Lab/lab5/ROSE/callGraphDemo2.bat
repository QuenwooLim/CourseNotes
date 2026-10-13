@echo off
cd bin
java -classpath .;..\lib\jgraph.jar;..\lib\callgraph.jar CallGraphDemo2
cd ..
@echo on
