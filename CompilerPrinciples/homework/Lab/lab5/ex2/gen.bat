@echo off
REM Generate OberonScanner.java from oberon.flex using JFlex
java -jar jflex\lib\jflex-full-1.8.2.jar src\oberon.flex
echo OberonScanner.java generated.
