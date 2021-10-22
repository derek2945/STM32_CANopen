#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "can.h" 
#include "canfestival.h"
 
 
/************************************************
* Azai测试CANopen使用
************************************************/
 
//void setTimer(void);

 int main(void)
 {	 

	u8 mode=CAN_Mode_Normal;//CAN工作模式;CAN_Mode_Normal(0)：普通模式，CAN_Mode_LoopBack(1)：环回模式
  u8 key;
  u8 canbuf[8];
   
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
//	LCD_Init();			   	//初始化LCD	
	KEY_Init();				//按键初始化		 	
   
  printf("Hello,Azai!\n");
   
//	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN初始化环回模式,波特率500Kbps    
  CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,2,mode);//CAN普通模式初始化, 波特率1000Kbps 

 	while(1)
	{
    key=KEY_Scan(0);
    if(key==KEY0_PRES)//KEY0按下,发送一次数据
		{
      canbuf[0]=0x01;
      canbuf[1]=0x02;
      canbuf[2]=0x03;
      canbuf[3]=0x04;
      Can_Send_Msg(canbuf,4);
       
    }
    
//    delay_ms(500);
//    printf("Hello,Azai!\n");

	}
}
 



