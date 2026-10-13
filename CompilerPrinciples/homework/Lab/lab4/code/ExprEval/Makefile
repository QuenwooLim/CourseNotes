# Makefile for ExprEval — matches .bat behavior
BIN := bin

.PHONY: build run doc test-simple test-standard test-all clean

build:
	cd src && javac -d ../$(BIN) -classpath ../$(BIN) parser/*.java

run: build
	cd $(BIN) && java ExprEval

doc: build
	cd src && javadoc -private -author -version -d ../doc -classpath ../$(BIN) -Xdoclint:none parser/*.java 2>&1

test-simple: build
	cd $(BIN) && java test.ExprEvalTest ../testcases/simple.xml

test-standard: build
	cd $(BIN) && java test.ExprEvalTest ../testcases/standard.xml > ../testcases/report.txt
	cat testcases/report.txt
	rm testcases/report.txt

test-my: build
	cd $(BIN) && java test.ExprEvalTest ../testcases/mytest.xml

test-all: test-simple test-standard test-my

clean:
	rm -rf $(BIN)/parser doc
