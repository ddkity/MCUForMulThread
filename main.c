/* ��Ƭ��ʵ����ѯ�������ķ�������ͬ�������ѯʱ��������� */
/* �˴�������8710Bn�� */

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
/*����Timer��ItvTime�Ĵ�С��ʾ��ѯ��ʱ�䣬��ֵ���ô�������ѯ��ʱ������һЩ */
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

/* 100ms��ʱ���ж� */
void timer1_timeout_handler(unsigned int TimeOut)
{
	TaskMark();
}

void main(void)
{
	gtimer_init(&MTimer1, TIMER2);
	/* 1ms����һ���ж� */
	gtimer_start_periodical(&MTimer1, 1000, (void*)timer1_timeout_handler, NULL);
	
	while ( 1 )
	{
		TaskProcess();//��������
	}
}



