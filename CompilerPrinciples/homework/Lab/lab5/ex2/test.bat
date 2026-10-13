@echo off
REM Run scanner on all test cases
echo === Testing correct program ===
java -cp bin ScannerTest ..\ex1\testcases\BankSystem.obr

echo.
echo === Testing lexical error variants ===
for %%f in (..\ex1\testcases\BankSystem.001 ..\ex1\testcases\BankSystem.002 ..\ex1\testcases\BankSystem.003 ..\ex1\testcases\BankSystem.004 ..\ex1\testcases\BankSystem.005) do (
    echo.
    echo --- %%f ---
    java -cp bin ScannerTest %%f
)

echo.
echo === Testing syntax error variants (should tokenize successfully) ===
for %%f in (..\ex1\testcases\BankSystem.006 ..\ex1\testcases\BankSystem.007 ..\ex1\testcases\BankSystem.008 ..\ex1\testcases\BankSystem.009 ..\ex1\testcases\BankSystem.010) do (
    echo.
    echo --- %%f ---
    java -cp bin ScannerTest %%f
)

echo.
echo === Testing semantic error variants (should tokenize successfully) ===
for %%f in (..\ex1\testcases\BankSystem.011 ..\ex1\testcases\BankSystem.012 ..\ex1\testcases\BankSystem.013) do (
    echo.
    echo --- %%f ---
    java -cp bin ScannerTest %%f
)
echo.
echo All tests completed.
