#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"
#include "memory.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;
// 内存管理器
MemoryManager memoryManager;

void wait_for_a_while() {
    for (volatile int i = 0; i < 1000000000; ++i); // 简单延时
}

void first_thread(void *arg) 
{
wait_for_a_while();
    printf("\n=========== Page Replacement Simulation Start ===========\n");

    char *p1 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 100);
    char *p2 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 10);
    char *p3 = (char *)memoryManager.allocatePages(AddressPoolType::KERNEL, 100);

    printf("Allocated Page Frames:\n");
    printf("  Frame 1: 0x%-10x\n", p1);
    printf("  Frame 2: 0x%-10x\n", p2);
    printf("  Frame 3: 0x%-10x\n", p3);
    wait_for_a_while();

    int pagelist[8] = {7, 3, 3, 7, 2, 1, 9, 8};
    int plist[3] = {0, 0, 0};
    int first = 0;
    int miss = 0;
    int hit = 0;
    int got = -1;
    char *pp;

    printf("\nChecking Page Access Sequence:\n");
    for (int i = 0; i < 8; i++) {
        printf("--------------------------------------------------\n");
        printf("Accessing Page: %d\n", pagelist[i]);

        int flag = 0;
        for (int j = 0; j < 3; j++) {
            if (plist[j] == pagelist[i]) {
                flag = 1;
                got = j;
            }
        }

        if (flag == 0) {
            printf("  Result     : MISS (Page Fault)\n");
            plist[first] = pagelist[i];
            printf("  Replaced   : Frame %d <- Page %d\n", first + 1, pagelist[i]);
            printf("  Frame State: [0x%x=%d] [0x%x=%d] [0x%x=%d]\n", 
                   p1, plist[0], p2, plist[1], p3, plist[2]);
            first = (first + 1) % 3;
            miss++;
        } else {
            if (got == 0) pp = p1;
            if (got == 1) pp = p2;
            if (got == 2) pp = p3;
            printf("  Result     : HIT (Found in Frame %d)\n", got + 1);
            printf("  Frame Addr : 0x%x\n", pp);
            hit++;
        }

        wait_for_a_while();  //每次处理完一页停顿一下
    }

    printf("==================================================\n");
    printf("Simulation Summary:\n");
    printf("  Total Accesses: %d\n", 8);
    printf("  Page Misses   : %d\n", miss);
    printf("  Page Hits     : %d\n", hit);
    printf("==================================================\n");

    asm_halt();
}


extern "C" void setup_kernel()
{

    // 中断管理器
    interruptManager.initialize();
    interruptManager.enableTimeInterrupt();
    interruptManager.setTimeInterrupt((void *)asm_time_interrupt_handler);

    // 输出管理器
    stdio.initialize();

    // 进程/线程管理器
    programManager.initialize();

    // 内存管理器
    memoryManager.openPageMechanism();
    memoryManager.initialize();

    // 创建第一个线程
    int pid = programManager.executeThread(first_thread, nullptr, "first thread", 1);
    if (pid == -1)
    {
        printf("can not execute thread\n");
        asm_halt();
    }

    ListItem *item = programManager.readyPrograms.front();
    PCB *firstThread = ListItem2PCB(item, tagInGeneralList);
    firstThread->status = RUNNING;
    programManager.readyPrograms.pop_front();
    programManager.running = firstThread;
    asm_switch_thread(0, firstThread);

    asm_halt();
}
