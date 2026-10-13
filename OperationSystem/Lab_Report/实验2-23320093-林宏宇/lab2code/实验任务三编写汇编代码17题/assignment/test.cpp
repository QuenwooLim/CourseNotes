#include <iostream>

extern "C" void student_main();  // 声明 student.asm 里的 student_main

int a1 = 10;  // 可以修改
int a2 = 15;  // 可以修改

void student_setting() {
    a1 = 10;
    a2 = 15;
}

int main() {
    student_setting();  // 设置变量
    student_main();     // 调用汇编函数
    return 0;
}

