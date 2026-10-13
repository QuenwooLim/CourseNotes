@echo off
REM Generate Parser.java and Sym.java from oberon.cup using JavaCUP 0.11b
java -jar javacup\java-cup-11b.jar -parser Parser -symbols Sym -destdir src src\oberon.cup
echo CUP generation complete. Output: src\Parser.java, src\Sym.java
