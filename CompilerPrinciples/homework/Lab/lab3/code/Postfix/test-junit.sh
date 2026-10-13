#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
LIB_DIR="${ROOT_DIR}/lib"
BIN_DIR="${ROOT_DIR}/bin"
JUNIT_JAR="${LIB_DIR}/junit-platform-console-standalone.jar"

# Run JUnit tests
echo "Running JUnit tests..."
java -jar "${JUNIT_JAR}" execute --class-path "${BIN_DIR}" --select-class PostfixTest
