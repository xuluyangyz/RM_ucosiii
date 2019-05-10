/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : YS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include <string.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

//OS_MEM  mem;                    //声明内存管理对象
//uint8_t ucArray [ 3 ] [ 20 ];   //声明内存分区大小


/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;    //任务控制块

static  OS_TCB   AppTaskLed1TCB;
static  OS_TCB   AppTaskLed2TCB;
static  OS_TCB   AppTaskLed3TCB;




/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];       //任务堆栈

static  CPU_STK  AppTaskLed1Stk [ APP_TASK_LED1_STK_SIZE ];
static  CPU_STK  AppTaskLed2Stk [ APP_TASK_LED2_STK_SIZE ];
static  CPU_STK  AppTaskLed3Stk [ APP_TASK_LED3_STK_SIZE ];



/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);               //任务函数声明

static  void  AppTaskLed1  ( void * p_arg );
static  void  AppTaskLed2  ( void * p_arg );
static  void  AppTaskLed3  ( void * p_arg );


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/
	OS_TICK			start;

int  main (void)
{
    OS_ERR  err;

	
    OSInit(&err);                                                           //初始化 uC/OS-III

	  /* 创建起始任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //任务控制块地址
                 (CPU_CHAR   *)"App Task Start",                            //任务名称
                 (OS_TASK_PTR ) AppTaskStart,                               //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //任务的优先级
                 (CPU_STK    *)&AppTaskStartStk[0],                         //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型

    OSStart(&err);                                                          //启动多任务管理（交由uC/OS-III控制）

}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


    (void)p_arg;

    CPU_Init();                                                 //初始化 CPU 组件（时间戳、关中断时间测量和主机名）
    BSP_Init();                                                 //板级初始化

    cpu_clk_freq = BSP_CPU_ClkFreq();                           //获取 CPU 内核时钟频率（SysTick 工作时钟）
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        //根据用户设定的时钟节拍频率计算 SysTick 定时器的计数值
    OS_CPU_SysTickInit(cnts);                                   //调用 SysTick 初始化函数，设置定时器计数值和启动定时器

    Mem_Init();                                                 //初始化内存管理组件（堆内存池和内存池表）

#if OS_CFG_STAT_TASK_EN > 0u                                    //如果使能（默认使能）了统计任务
    OSStatTaskCPUUsageInit(&err);                               //计算没有应用任务（只有空闲任务）运行时 CPU 的（最大）
#endif                                                          //容量（决定 OS_Stat_IdleCtrMax 的值，为后面计算 CPU 
                                                                //使用率使用）。
#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                //复位（清零）当前最大关中断时间
#endif

    
//    /* 配置时间片轮转调度 */		
//    OSSchedRoundRobinCfg((CPU_BOOLEAN   )DEF_ENABLED,          //使能时间片轮转调度
//		                     (OS_TICK       )0,                    //把 OSCfg_TickRate_Hz / 10 设为默认时间片值
//												 (OS_ERR       *)&err );               //返回错误类型


		/* 创建 LED1 任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskLed1TCB,                             //任务控制块地址
                 (CPU_CHAR   *)"App Task Led1",                             //任务名称
                 (OS_TASK_PTR ) AppTaskLed1,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_LED1_PRIO,                         //任务的优先级
                 (CPU_STK    *)&AppTaskLed1Stk[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
		
		/* 创建 LED2 任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskLed2TCB,                             //任务控制块地址
                 (CPU_CHAR   *)"App Task Led2",                             //任务名称
                 (OS_TASK_PTR ) AppTaskLed2,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_LED2_PRIO,                         //任务的优先级
                 (CPU_STK    *)&AppTaskLed2Stk[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型								 

		/* 创建 LED3 任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskLed3TCB,                             //任务控制块地址
                 (CPU_CHAR   *)"App Task Led3",                             //任务名称
                 (OS_TASK_PTR ) AppTaskLed3,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_LED3_PRIO,                         //任务的优先级
                 (CPU_STK    *)&AppTaskLed3Stk[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
								 
	
		OSTaskDel ( 0, & err );                     //删除起始任务本身，该任务不再运行

		
}


/*
*********************************************************************************************************
*                                          LED1 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed1 ( void * p_arg )
{
	
	OS_ERR      err;
	OS_TICK			begain=0;
	OS_TICK			finish=0;
	OS_TICK			i=0;
	OS_TICK			dly=0;
	OS_REG      value;	
	(void)p_arg;
	
	start = OSTimeGet( &err );			//系统上电后开始执行用户任务的其实时间
	
	while(1){
	begain = OSTimeGet(&err);													//记录任务进入的时间
	value  = OSTaskRegGet ( 0, 0, & err );	
		while ( value<1 )				//value记录周期数，亮灭依次表示消耗一个周期，一个周期为100个tick
		{
			LED1_ON ; 
			OSTaskRegSet(0,0,++value,&err);
			OSIntEnter();							//禁止任务调度，保证以下代码为一个时间单位
			while(i<=14999999)							//模拟任务执行消耗时间片
				i++;
				i=0;
			
			LED1_OFF ;
			while(i<=14999999)							//模拟任务执行消耗时间片
				i++;
				i=0;
			OSIntExit();

		}	
		
		OSTaskRegSet ( 0, 0, 0, & err );
		finish=OSTimeGet(&err);	
			if (400!=(finish-start))
				dly=400-(finish-start)%400;
			else
				dly=0;
		OSTimeDly(dly, OS_OPT_TIME_DLY, & err );	//400-(finish-begain)
		
}
}

/*
*********************************************************************************************************
*                                          LED2 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed2 ( void * p_arg )
{
	OS_ERR      err;
	OS_TICK			begain=0;
	OS_TICK			finish=0;
	OS_TICK			i=0;
	OS_TICK			dly;
	OS_REG      value;	
	(void)p_arg;
	begain=OSTimeGet(&err);
	while(1){										
	
		value = OSTaskRegGet ( 0, 0, & err );	
		while ( value<2 )				//value记录消耗时间片数
		{
			OSTaskRegSet(0,0,++value,&err);
			OSIntEnter();
			LED2_ON ; 
			while(i<=14999999)							//模拟任务执行消耗时间片
				i++;
				i=0;
			
			LED2_OFF ; 
			while(i<=14999999)							//模拟任务执行消耗时间片
				i++;
				i=0;
			OSIntExit();
		}
		
			OSTaskRegSet ( 0, 0, 0, & err );
			finish=OSTimeGet(&err);
			if (600!=(finish-start))
				dly=600-(finish-start)%600;
			else
				dly=0;
			OSTimeDly(dly, OS_OPT_TIME_DLY, & err );		                        
		
}	
}


/*
*********************************************************************************************************
*                                          LED3 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed3 ( void * p_arg )
{
	OS_ERR      err;
	OS_TICK			begain;
	OS_TICK			finish;
	OS_TICK			i=0;
	OS_TICK			dly;
	OS_REG      value;	
	(void)p_arg;
	
	while(1){
	begain=OSTimeGet(&err);													//记录任务进入的时间

		value = OSTaskRegGet ( 0, 0, & err );			
		while ( value<3 )				//value记录消耗时间片数
		{
			//OS_ENTER_CRITICAL();
			OSIntEnter();
			LED3_ON ;
			while(i<=14999999)							//模拟任务执行消耗时间片
				i++;
				i=0;
			
			LED3_OFF ;
			while(i<=14999999)							//模拟任务执行消耗时间片
				i++;
				i=0;
			OSIntExit();
			//OS_EXIT_CRITICAL();
			OSTaskRegSet(0,0,++value,&err);		//value值加1
		}

			OSTaskRegSet ( 0, 0, 0, & err );
			finish=OSTimeGet(&err);	
			dly=finish-start;
	/**如果任务正好在任务周期末尾执行完，则dly的值为一个任务周期 ，此处加一个判断 **/
			if (dly!=1000)		
				dly=1000-(finish-start)%1000;
			else
				dly=0;
			OSTimeDly(dly, OS_OPT_TIME_DLY, & err );
			                        	
}
}
	


