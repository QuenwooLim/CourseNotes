@echo off
@echo Running Testcase 002: a correct long input.
@echo ==============================================
@echo The input is:
type testcases\tc-002.infix
@echo ----------------------------------------------
cd bin

rem : Run the testcase with input direction
java Postfix < ..\testcases\tc-002.infix

rem : Compare the expected output
@echo ----------------------------------------------
@echo The output should be: 
type ..\testcases\tc-002.postfix

cd ..
@echo ==============================================
pause
@echo on
