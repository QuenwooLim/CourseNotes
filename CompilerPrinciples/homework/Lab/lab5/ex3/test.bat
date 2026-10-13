@echo off
REM Run parser on all test cases
echo === Testing correct program ===
java -cp bin;lib/callgraph.jar;lib/jgraph.jar OberonParser ..\ex1\testcases\BankSystem.obr --no-gui

echo.
echo === Testing syntax/semantic error variants ===
for %%f in (..\ex1\testcases\BankSystem.006 ..\ex1\testcases\BankSystem.007 ..\ex1\testcases\BankSystem.008 ..\ex1\testcases\BankSystem.009 ..\ex1\testcases\BankSystem.010 ..\ex1\testcases\BankSystem.011 ..\ex1\testcases\BankSystem.012 ..\ex1\testcases\BankSystem.013) do (
    echo.
    echo --- %%f ---
    java -cp bin;lib/callgraph.jar;lib/jgraph.jar OberonParser %%f --no-gui
)
echo.
echo All tests completed.
