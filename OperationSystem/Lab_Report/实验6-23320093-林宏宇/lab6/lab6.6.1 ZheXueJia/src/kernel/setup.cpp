#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"
#define DELAY 0x0ffffff

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;

Semaphore avil_1, avil_2, avil_3, avil_4, avil_5;

void wait(int delay = DELAY){
	while (delay)
		--delay;
}

void a1(void *arg)
{
	for(int e = 0; e < 20; e++){
		avil_1.P();
		wait(); 
		avil_5.P();
		printf("1   ");
		wait();
		avil_5.V();
		avil_1.V();
	}
}

void a2(void *arg)
{
	for(int e = 0; e < 20; e++){
	   	avil_2.P();
		wait();
	   	avil_1.P();
		printf("2   ");
		wait();
		avil_1.V();
		avil_2.V();
	}
}

void a3(void *arg)
{
	for(int e = 0; e < 20; e++){
	   	avil_3.P();
		wait();
	   	avil_2.P();
		printf("3   ");
		wait();
		avil_2.V();
		avil_3.V();
	}
}

void a4(void *arg)
{
	for(int e = 0; e < 20; e++){
	   	avil_4.P();
		wait();
	   	avil_3.P();
		printf("4   ");
		wait();
		avil_3.V();
		avil_4.V();
	}
}

void a5(void *arg)
{
	for(int e = 0; e < 20; e++){
	   	avil_5.P();
		wait();
	   	avil_4.P();
		printf("5   ");
		wait();
		avil_4.V();
		avil_5.V();
	}
}

void first_thread(void *arg)
{
    // 第1个线程不可以返回
    stdio.moveCursor(0);
    for (int i = 0; i < 25 * 80; ++i)
    {
        stdio.print(' ');
    }
    stdio.moveCursor(0);

    avil_1.initialize(1);
    avil_2.initialize(1);
    avil_3.initialize(1);
    avil_4.initialize(1);
    avil_5.initialize(1);

    programManager.executeThread(a1, nullptr, "1 thread", 1);
    programManager.executeThread(a2, nullptr, "2 thread", 1);
    programManager.executeThread(a3, nullptr, "3 thread", 1);
    programManager.executeThread(a4, nullptr, "4 thread", 1);
    programManager.executeThread(a5, nullptr, "5 thread", 1);
    printf("23320093:");
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
