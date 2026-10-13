#!/usr/bin/env bash
set -euo pipefail

mkdir -p bin
echo "Compiling source files..."
# Compile all source files so Benchmark, PostfixRecoverable and others are available
javac -d bin src/*.java

JUNIT_JAR="lib/junit-platform-console-standalone.jar"
if [ -f "$JUNIT_JAR" ]; then
	echo "Compiling test sources with JUnit on classpath..."
	javac -cp "bin:$JUNIT_JAR" -d bin test/*.java
else
	echo "Warning: $JUNIT_JAR not found — skipping test compilation."
fi