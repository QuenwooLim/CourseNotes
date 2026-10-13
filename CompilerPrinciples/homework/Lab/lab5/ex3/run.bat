@echo off
REM Run parser on the correct Oberon-0 test program
java -cp bin;lib/callgraph.jar;lib/jgraph.jar OberonParser ..\ex1\testcases\BankSystem.obr
