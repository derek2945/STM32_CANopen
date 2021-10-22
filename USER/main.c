#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "can.h" 
#include "canfestival.h"
 
 
/************************************************
* Azai����CANopenʹ��
************************************************/
 
//void setTimer(void);

 int main(void)
 {	 

	u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ
  u8 key;
  u8 canbuf[8];
   
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
//	LCD_Init();			   	//��ʼ��LCD	
	KEY_Init();				//������ʼ��		 	
   
  printf("Hello,Azai!\n");
   
//	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������500Kbps    
  CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,2,mode);//CAN��ͨģʽ��ʼ��, ������1000Kbps 

 	while(1)
	{
    key=KEY_Scan(0);
    if(key==KEY0_PRES)//KEY0����,����һ������
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
 



