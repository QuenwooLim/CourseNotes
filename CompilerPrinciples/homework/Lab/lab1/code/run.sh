#!/bin/sh
# 进入当前脚本所在目录
cd "$(dirname "$0")"

# 编译所有Java文件（需提前下载junit-4.13.2.jar和hamcrest-core-1.3.jar放在当前目录）
javac -cp .:junit-4.13.2.jar:hamcrest-core-1.3.jar *.java

echo "\n 启动个人所得税计算器..."
java PersonalTaxApp

echo "\n 运行 JUnit 单元测试..."
java -cp .:junit-4.13.2.jar:hamcrest-core-1.3.jar org.junit.runner.JUnitCore TaxCalculatorTest
