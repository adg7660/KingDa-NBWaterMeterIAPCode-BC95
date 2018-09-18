/*********************************************************************************
//概述：
//作者：随风飘跃     时间：       地点：
//CPU型号：         系统主频：
//版本号：V0.0        
*********************************************************************************/
/*********************************************************************************
文件包含区
*********************************************************************************/
#include "include.h"
/*********************************************************************************
常量定义区
*********************************************************************************/
/*********************************************************************************
公共变量定义区
*********************************************************************************/
/*********************************************************************************
外部变量声明区
*********************************************************************************/
/*********************************************************************************
私有变量定义区
*********************************************************************************/ 
/*********************************************************************************
测试变量定义区
*********************************************************************************/
/*********************************************************************************
内部函数定义区
*********************************************************************************/
/*********************************************************************************
功能代码定义区
*********************************************************************************/

/*********************************************************************************
功能代码定义区
*********************************************************************************/
/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void main(void)
{ 
  disableInterrupts();                                      //关总中断
  
  STM8_Interrupt_Vector_Table_Init();
  Check_Run_APP();
  
  RCC_Configuration();
  GPIO_Configuration();
  Rtc_Config();
  Pulse_Acquire_Config();
  IWDG_INIT(); 
  enableInterrupts();                                       //开总中断
/////////////////////////////////////////////////////////    
  Read_Meter_Parameter();
  Read_ACUM_Flow(ADD_FLOW_ADD,&Cal.Water_Data);         //读取当前累积水量

  BC95.Report_Bit = 1;
  BC95.Start_Process = BC95_RECONNECT;

  while (1)
  {
//    RTC_GetDate(RTC_Format_BIN, &RTC_DateStr);
//    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStr);
    
    IWDG_ReloadCounter();//重载看门狗计数器
    
    //上报失败2次则复位
    if( BC95.FailTimes >= 2 )
    { 
      Save_Add_Flow(ADD_FLOW_ADD,&Cal.Water_Data);       //保存当前水量
      WWDG->CR = 0x80;  //看门狗复位
    }
    
    if(MeterParameter.DeviceStatus == SLEEP)     //设备进入睡眠状态
    {
      if(APPValid == APP_VALID)      //如果APP程序有效则运行APP程序
      {
        Save_Add_Flow(ADD_FLOW_ADD,&Cal.Water_Data);
        Run_APP();
      }
      else      //否则睡眠
      {
        Sleep();
      }
    } 
    else
    {
      Sys_Timer_Process();
      BC95_Process(); 
    }
  }

}

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void Sleep(void)
{  
  BC95.Start_Process = BC95_POWER_DOWN;
  BC95.Report_Bit = 0;
  MeterParameter.DeviceRunTiming = 0;
  
  

//  CLK_HaltConfig(CLK_Halt_FastWakeup,ENABLE);   //快速唤醒后时钟为HSI  
//  PWR_FastWakeUpCmd(ENABLE);                    //开启电源管理里的快速唤醒  
  PWR_UltraLowPowerCmd(ENABLE);                 //使能电源的低功耗模式          //开启后内部参考电压获取值变小
//  CLK_HSICmd(DISABLE);                          //关闭内部高速时钟

  halt();
}
/**
*******************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/
void IWDG_INIT(void)  //看门狗初始化
{ 
  IWDG_Enable();//启动看门狗
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//允许写预分频器和重载寄存器
  IWDG_SetPrescaler(IWDG_Prescaler_256);//设置IWDG预分频值
  IWDG_SetReload(0xFF);//设置重载值1.7s：(255+1)*256/38K = 1.72s
  IWDG_ReloadCounter();//重载计数器
}

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

/*********************************************************************************
 Function:      //
 Description:   //
 Input:         //
                //
 Output:        //
 Return:      	//
 Others:        //
*********************************************************************************/

