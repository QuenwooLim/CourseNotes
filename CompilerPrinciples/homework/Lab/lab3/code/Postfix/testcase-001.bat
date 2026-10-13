@echo off
@echo Running Testcase 001: a correct input from DBv2.
@echo ==============================================
@echo The input is:
type testcases\tc-001.infix
@echo ----------------------------------------------
cd bin

rem : Run the testcase with input direction
java Postfix < ..\testcases\tc-001.infix

rem : Compare the expected output
@echo ----------------------------------------------
@echo The output should be: 
type ..\testcases\tc-001.postfix

cd ..
@echo ==============================================
pause
@echo on
