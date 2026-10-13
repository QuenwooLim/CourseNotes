#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"

// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;

Semaphore semaphore;

int matcha_count;
int mango_count;
int plate_capacity;

void man(void *arg)
{
	semaphore.P();
	int delay=0;
	if (matcha_count == 0)
	{
		if(matcha_count + mango_count < plate_capacity)
		{
		    delay = 0xfffffff;
		    while (delay)
			--delay;
			matcha_count = matcha_count + 1;
			printf(" %d ", matcha_count);

		}	
	}
	else
	{
		matcha_count = matcha_count - 1;
		printf(" %d ", matcha_count);
	}
	semaphore.V();
}
void woman(void *arg)
{
	semaphore.P();
	int delay=0;
	if (mango_count == 0)
	{
		if(matcha_count + mango_count < plate_capacity)
		{
		    delay = 0xfffffff;
		    while (delay)
			--delay;
			mango_count = mango_count + 1;
			printf(" %d ", mango_count);
		}	
	}
	else
	{ 
		mango_count = mango_count - 1;
		printf(" %d ", mango_count);
	}
	semaphore.V();
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
	
    //Start here

    //Initial
    matcha_count = 0;
    mango_count = 0;
    plate_capacity = 5;
    semaphore.initialize(5);//5 cakes

    programManager.executeThread(man, nullptr, "man thread", 1);
    programManager.executeThread(man, nullptr, "man thread", 1);
    programManager.executeThread(man, nullptr, "man thread", 1);
    programManager.executeThread(man, nullptr, "man thread", 1);
    programManager.executeThread(man, nullptr, "man thread", 1);
    programManager.executeThread(man, nullptr, "man thread", 1);

    programManager.executeThread(woman, nullptr, "woman thread", 1);
    programManager.executeThread(woman, nullptr, "woman thread", 1);
    programManager.executeThread(woman, nullptr, "woman thread", 1);
    programManager.executeThread(woman, nullptr, "woman thread", 1);

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
