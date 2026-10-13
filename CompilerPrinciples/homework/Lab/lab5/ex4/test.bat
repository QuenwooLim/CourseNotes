@echo off
echo === Testing correct program ===
java -cp bin;lib/flowchart.jar;lib/jgraph.jar OberonParser ..\ex1\testcases\BankSystem.obr --no-gui
echo.
echo === Testing error variants ===
for %%f in (..\ex1\testcases\BankSystem.006 ..\ex1\testcases\BankSystem.007 ..\ex1\testcases\BankSystem.008 ..\ex1\testcases\BankSystem.009 ..\ex1\testcases\BankSystem.010 ..\ex1\testcases\BankSystem.013) do (
    echo --- %%f ---
    java -cp bin;lib/flowchart.jar;lib/jgraph.jar OberonParser %%f --no-gui
)
echo All tests completed.
