@echo off
@echo Running Testcase 003: missing an operator.
@echo ==============================================
@echo The input is:
type testcases\tc-003.infix
@echo ----------------------------------------------
cd bin

rem : Run the testcase with input direction
java Postfix < ..\testcases\tc-003.infix

rem : Compare the expected output
@echo ----------------------------------------------
@echo The output should be: 
type ..\testcases\tc-003.postfix

cd ..
@echo ==============================================
pause
@echo on
