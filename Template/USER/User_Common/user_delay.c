#include "main.h"
/******************************************************************
函数名称：
函数功能:US延时
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void user_delayUs(int time)
{
  for (int i = 0; i < time; i++)
  {
    for (int j = 0; j < 24; j++) // 1us
    {
      
    }
  }
}
/******************************************************************
函数名称：
函数功能:MS延时
输入参数：
输出参数：
返回值：
注意：
******************************************************************/
/******************************************************************
函数更改历史记录：
******************************************************************/
void user_delayMs(int time)
{
  for (int i = 0; i < 1000; i++)
  {
    user_delayUs(time);
  }
}



/**********************************************************************
 * @ 函数名  ： delay_us
 * @ 功能说明： us延时，器件关闭OS调度
 * @ 参数    ：   
 * @ 返回值  ： 无
********************************************************************/
extern void vPortSetupTimerInterrupt( void );
void delay_us(uint32_t nus)
{ 
       uint32_t ticks;
       uint32_t told,tnow,reload,tcnt=0;
       if((0x0001&(SysTick->CTRL)) ==0)    //定时器未工作
       vPortSetupTimerInterrupt();         //初始化定时器 
       reload = SysTick->LOAD;                     //获取重装载寄存器值
       ticks = nus * (SystemCoreClock / 1000000);  //计数时间值
       
       vTaskSuspendAll();//阻止OS调度，防止打断us延时
       told=SysTick->VAL;  //获取当前数值寄存器值（开始时数值）
       while(1)
       {
              tnow=SysTick->VAL; //获取当前数值寄存器值
              if(tnow!=told)  //当前值不等于开始值说明已在计数
              {         
                     if(tnow<told)  //当前值小于开始数值，说明未计到0
                          tcnt+=told-tnow; //计数值=开始值-当前值
 
                     else     //当前值大于开始数值，说明已计到0并重新计数
                            tcnt+=reload-tnow+told;   //计数值=重装载值-当前值+开始值  （
                                                      //已从开始值计到0） 
 
                     told=tnow;   //更新开始值
                     if(tcnt>=ticks)break;  //时间超过/等于要延迟的时间,则退出.
              } 
       }  
       xTaskResumeAll();	//恢复OS调度		   
} 

/**********************************************************************
 * @ 函数名  ： delay_ms
 * @ 功能说明： ms延时，器件关闭OS调度
 * @ 参数    ：   
 * @ 返回值  ： 无
********************************************************************/
void delay_ms(uint16_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++){
	  delay_us(1000);
	}

}



/**********************************************************************
 * @ 函数名  ： DWT_Init
 * @ 功能说明： DWT_Init初始化，使用DWT进行延时需要先初始化
 * @ 参数    ：   
 * @ 返回值  ： 无
********************************************************************/
void DWT_Init(void)
{
    DEM_CR  |=  DEM_CR_TRCENA; /*对DEMCR寄存器的位24控制，写1使能DWT外设。*/
    DWT_CYCCNT = 0;/*对于DWT的CYCCNT计数寄存器清0。*/
    DWT_CR  |=  DWT_CR_CYCCNTENA;/*对DWT控制寄存器的位0控制，写1使能CYCCNT寄存器。*/
}

/**********************************************************************
 * @ 函数名  ： DWT_DelayUS
 * @ 功能说明： DWT_DelayUS us级延时
 * @ 参数    ：   
 * @ 返回值  ： 无
********************************************************************/
void DWT_DelayUS(uint32_t _ulDelayTime)
{
    uint32_t tCnt, tDelayCnt;
    uint32_t tStart;
           
    tStart = DWT_CYCCNT; /* 刚进入时的计数器值 */
    tCnt = 0;
    tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);
    /* 需要的节拍数 */    /*SystemCoreClock :系统时钟频率*/                 

    while(tCnt < tDelayCnt)
      {
        tCnt = DWT_CYCCNT - tStart; 
        /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */       
      }
}

/**********************************************************************
 * @ 函数名  ： DWT_DelayMS
 * @ 功能说明： DWT_DelayMS ms级延时
 * @ 参数    ：   
 * @ 返回值  ： 无
********************************************************************/
void DWT_DelayMS(uint32_t _ulDelayTime)
{
     DWT_DelayUS(1000*_ulDelayTime);
}