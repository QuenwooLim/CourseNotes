@echo off
REM Compile Oberon-0 parser (Experiment 3)
javac -cp lib/callgraph.jar;lib/jgraph.jar -d bin src/exceptions/*.java src/Token.java src/OberonScanner.java src/OberonParser.java
echo Build complete.
