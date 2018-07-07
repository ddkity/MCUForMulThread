/* 单片机实现轮询多个任务的方法，不同任务的轮询时间可以设置 */
/* 此代码跑在8710Bn上 */

#include "device.h"
#include "gpio_api.h"   // mbed
#include "main.h"
#include "timer_api.h"

#define  true				(1)
#define  false				(0)
#define TASKS_MAX   (6)

gtimer_t MTimer1;

typedef struct TASK_COMPONENTS
{
    bool Run;
    unsigned char Timer;
    unsigned char ItvTime;
    void( *TaskHook )( void );
} TASK_COMPONENTS;

void func0(void)
{
	printf("func0.\n");
}

void func1(void)
{
	printf("func1.\n");
}

void func2(void)
{
	printf("func2.\n");
}

void func3(void)
{
	printf("func3.\n");
}

void func4(void)
{
	printf("func4.\n");
}

void func5(void)
{
	printf("func5.\n");
}
/*设置Timer和ItvTime的大小表示轮询的时间，数值设置大了则轮询的时间间隔大一些 */
TASK_COMPONENTS Task[TASKS_MAX] = {\
	{false, 5, 5, func0}, \
	{false, 10, 10, func1}, \
	{false, 15, 15, func2},\
	{false, 20, 20, func3},\
	{false, 25, 25, func4},\
	{false, 30, 30, func5},
};

void TaskProcess( void )
{
	unsigned char i ;

	for ( i = 0; i < TASKS_MAX; i++ )
	{
		if ( Task[i].Run == true )
		{
			Task[i].TaskHook();
			Task[i].Run = false;
		}
	}
}

void TaskMark( void )
{
	unsigned char i ;

	for ( i = 0; i < TASKS_MAX; i++ )
	{
		if ( Task[i].Timer != 0 )
		{
			Task[i].Timer--;
			if ( 0 == Task[i].Timer )
			{
				Task[i].Timer = Task[i].ItvTime;
				Task[i].Run = true;
			}
		}
	}
}

/* 100ms定时器中断 */
void timer1_timeout_handler(unsigned int TimeOut)
{
	TaskMark();
}

void main(void)
{
	gtimer_init(&MTimer1, TIMER2);
	/* 1ms进入一次中断 */
	gtimer_start_periodical(&MTimer1, 1000, (void*)timer1_timeout_handler, NULL);
	
	while ( 1 )
	{
		TaskProcess();//任务处理函数
	}
}



