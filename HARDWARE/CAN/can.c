#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "canfestival.h"
						  
//////////////////////////////////////////////////////////////////////////////////
//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024;  tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//������Ϊ:36M/((8+9+1)*4)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��;


/* CANOPEN�ֵ� */
extern CO_Data masterObjdict_Data; 

u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

	  GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
 	  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//��ʼ��IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��IO
	  
 	//CAN��Ԫ����
 	  CAN_InitStructure.CAN_TTCM=DISABLE;						 //��ʱ�䴥��ͨ��ģʽ  //
 	  CAN_InitStructure.CAN_ABOM=DISABLE;						 //����Զ����߹���	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 	//��ֹ�����Զ����� //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //���Ĳ�����,�µĸ��Ǿɵ� // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //���ȼ��ɱ��ı�ʶ������ //
  	CAN_InitStructure.CAN_Mode= mode;	         //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; //
  	//���ò�����
  	CAN_InitStructure.CAN_SJW=tsjw;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //��Ƶϵ��(Fdiv)Ϊbrp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // ��ʼ��CAN1 

 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
   	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
 	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0

  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
#if CAN_RX0_INT_ENABLE
	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
//  u32 i;
//    CanRxMsg RxMessage;
//    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
//    for(i=0;i<RxMessage.DLC;i++)
//    {
//      printf("%02x\n",RxMessage.Data[i]);
//    }   
  unsigned int i = 0;
	Message RxMSG = Message_Initializer;
	CANOpen_Message CAN1_Rx_m;

	printf("can1_RX0_IRQHandler\r\n");
	
	CAN_Receive(CAN1, CAN_FIFO0, &(CAN1_Rx_m.m));	        //��CAN1 FIFO0��������
	RxMSG.cob_id = (uint16_t)(CAN1_Rx_m.m.StdId);
	RxMSG.rtr = CAN1_Rx_m.m.RTR;
	RxMSG.len = CAN1_Rx_m.m.DLC;
	printf("R:");
	printf("ID=%04x--->",	RxMSG.cob_id);
	
	for(i=0;i<RxMSG.len;i++)
	{
		   RxMSG.data[i] = CAN1_Rx_m.m.Data[i];		
			 printf("%02x",RxMSG.data[i]);		
	}
	printf("\r\n");

//	canDispatch(CANObject, &(RxMSG));
}
#endif

/**
  * @brief  canSend
	* @param  CANx:CAN1 or CAN2   m:can message
  * @retval 0��Success
  */
unsigned char canSend(CAN_PORT CANx, Message *m)	                
{
		unsigned char ret;
		unsigned char i;
		CanTxMsg TxMessage;
	
		printf("canSend\r\n");
	
		TxMessage.StdId = (uint32_t)(m->cob_id);
		TxMessage.ExtId = 0x00;
		TxMessage.RTR = m->rtr;		                   //����֡						  
		TxMessage.IDE = CAN_ID_STD;                  //��׼֡        
		TxMessage.DLC = m->len;                      //���ݳ���        
	  printf("S:");	
		printf("ID=%04x  RTR=%d  |  data-->",	TxMessage.StdId,	TxMessage.RTR);
		for(i=0;i<m->len;i++)                                 
		{
				TxMessage.Data[i] = m->data[i];
				printf("%02x  ",m->data[i]);	
		}
		printf("\r\n");
		ret = CAN_Transmit(CANx, &TxMessage);
		if(ret == CAN_TxStatus_NoMailBox)
		{
				return 1;	
		}
		else 
		{
				return 0;    
		}
}

// can ���պ��� 
unsigned char CAN1_Receive_Msg(Message *RxMSG)
{		   		   
		u32 i;
	  //Message RxMSG = Message_Initializer;
	  CANOpen_Message CAN1_Rx_m;
	  if( CAN_MessagePending(CAN1,CAN_FIFO0)==0) return 0;		//û�н��յ�����,ֱ���˳� 
		CAN_Receive(CAN1, CAN_FIFO0, &(CAN1_Rx_m.m));	        //��CAN1 FIFO0��������
		RxMSG->cob_id = (uint16_t)(CAN1_Rx_m.m.StdId);
		RxMSG->rtr = CAN1_Rx_m.m.RTR;
		RxMSG->len = CAN1_Rx_m.m.DLC;
		for(i=0;i<RxMSG->len;i++)
		{
			RxMSG->data[i] = CAN1_Rx_m.m.Data[i];
		}
 
  	 return RxMSG->len;	
}





//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 Can_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x12;					 // ��׼��ʶ�� 
  TxMessage.ExtId=0x12;				   // ������չ��ʾ�� 
  TxMessage.IDE=CAN_Id_Standard; // ��׼֡
  TxMessage.RTR=CAN_RTR_Data;		 // ����֡
  TxMessage.DLC=len;						// Ҫ���͵����ݳ���
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];			          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		

}

//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}














