@echo off
cd bin
java -classpath .;..\lib\jgraph.jar;..\lib\callgraph.jar CallGraphDemo1
cd ..
@echo on
