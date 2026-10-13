#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="${ROOT_DIR}/src"
DOC_DIR="${ROOT_DIR}/doc"

TEST_DIR="${ROOT_DIR}/test"
JUNIT_JAR="${ROOT_DIR}/lib/junit-platform-console-standalone.jar"

mkdir -p "${DOC_DIR}"

# Collect all Java source files under src and test (recursive), safe with spaces
FILES=()
while IFS= read -r -d '' f; do
	FILES+=("$f")
done < <(find "${SRC_DIR}" -type f -name '*.java' -print0)

if [ -d "${TEST_DIR}" ]; then
	# Collect test sources separately so we can decide whether to include them
	TEST_FILES=()
	while IFS= read -r -d '' f; do
		TEST_FILES+=("$f")
	done < <(find "${TEST_DIR}" -type f -name '*.java' -print0)
  
	if [ ${#TEST_FILES[@]} -gt 0 ]; then
		if [ -f "${JUNIT_JAR}" ]; then
			FILES+=("${TEST_FILES[@]}")
			CLASSPATH="${JUNIT_JAR}"
		else
			echo "Warning: junit jar not found at ${JUNIT_JAR}; skipping test sources in Javadoc."
		fi
	fi
fi

if [ ${#FILES[@]} -eq 0 ]; then
	echo "No Java source files found in ${SRC_DIR} or ${TEST_DIR}"
	exit 1
fi

echo "Generating Javadoc for sources under ${SRC_DIR}"$( [ -n "${CLASSPATH:-}" ] && echo " and ${TEST_DIR}" )"..."

if [ -n "${CLASSPATH:-}" ]; then
	javadoc -d "${DOC_DIR}" -classpath "${CLASSPATH}" "${FILES[@]}"
else
	javadoc -d "${DOC_DIR}" "${FILES[@]}"
fi
