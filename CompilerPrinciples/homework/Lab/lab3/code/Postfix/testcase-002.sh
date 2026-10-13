#!/usr/bin/env bash
set -euo pipefail

echo "Running Testcase 002: a correct long input."
echo "=============================================="
echo "The input is:"
cat testcases/tc-002.infix
echo "----------------------------------------------"
cd bin

# : Run the testcase with input direction
java Postfix < ../testcases/tc-002.infix || true

# : Compare the expected output
echo "----------------------------------------------"
echo "The output should be: "
cat ../testcases/tc-002.postfix

cd ..
echo "=============================================="
echo
