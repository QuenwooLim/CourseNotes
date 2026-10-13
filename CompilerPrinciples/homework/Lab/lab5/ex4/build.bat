@echo off
javac -cp lib/flowchart.jar;lib/jgraph.jar -d bin src/exceptions/*.java src/Token.java src/OberonScanner.java src/OberonRDParser.java
echo Build complete.
