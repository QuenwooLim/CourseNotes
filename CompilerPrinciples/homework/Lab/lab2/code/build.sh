#!/usr/bin/env bash
set -e
mkdir -p bin doc lib
# compile main java sources under src (exclude test classes)
main_sources=$(find src -name "*.java" ! -name "*Test.java" | tr '\n' ' ')
if [ -z "$main_sources" ]; then
	echo "No main Java sources found under src"
else
	javac -d bin $main_sources
	echo "Compile completed. Classes are in bin/"
fi

# generate javadoc into doc/
if command -v javadoc >/dev/null 2>&1; then
	javadoc -d doc -encoding UTF-8 -charset UTF-8 $sources || true
	echo "Javadoc generated in doc/"
else
	echo "javadoc not found; skipping doc generation"
fi

# JUnit test support: expect user to provide console-standalone jar in lib/
JUNIT_JAR=lib/junit-platform-console-standalone.jar
JUNIT_URL=https://repo1.maven.org/maven2/org/junit/platform/junit-platform-console-standalone/1.9.4/junit-platform-console-standalone-1.9.4.jar

# Find test sources under src and test (supports tests moved into src)
test_sources=$(find src test -name "*Test.java" 2>/dev/null | tr '\n' ' ')
if [ -n "$test_sources" ]; then
	echo "Found test sources. Ensure JUnit jar is present at $JUNIT_JAR to run tests."
	if [ ! -f "$JUNIT_JAR" ]; then
		echo "Skipping tests: $JUNIT_JAR not found."
		echo "Please download $JUNIT_URL and save it as $JUNIT_JAR"
	else
		echo "Compiling tests..."
		javac -cp "bin:$JUNIT_JAR" -d bin $test_sources
		echo "Running tests..."
		# Use the 'execute' subcommand which is required by newer junit console jars
		java -jar "$JUNIT_JAR" execute --scan-classpath --class-path bin
	fi
else
	echo "No test sources found (src/*Test.java or test/*Test.java)"
fi

echo "Build completed."
