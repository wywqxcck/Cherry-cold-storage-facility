/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, temp_duan, China, DY.
**                           All Rights Reserved
**
**                           By(�������ɿƼ����޹�˾)
**                             http://www.kpiot.top
**
**----------------------------------�ļ���Ϣ------------------------------------
** �ļ�����: usart1.c
** ������Ա: temp_duan
** ��������: 2019-08-01
** �ĵ�����: 
**
**----------------------------------�汾��Ϣ------------------------------------
** �汾����: V0.1
** �汾˵��: ��ʼ�汾
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "usart.h"
/*******************************************************************************
** ��������: usart1_init
** ��������: 
** ����˵��: bound: [����/��] 
** ����˵��: None
** ������Ա: temp_duan
** ��������: 2019-08-01
**------------------------------------------------------------------------------
** �޸���Ա:
** �޸�����:
** �޸�����:
**------------------------------------------------------------------------------
********************************************************************************/
void usart1_init(u32 bound)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ������ʱ��
    
    GPIO_InitTypeDef GPIO_Init_Struct;
    GPIO_Init_Struct.GPIO_Mode=GPIO_Mode_AF_PP;                      //TXDΪ�����������
    GPIO_Init_Struct.GPIO_Pin=GPIO_Pin_9;                            //9��ΪTXD
    GPIO_Init_Struct.GPIO_Speed=GPIO_Speed_50MHz;                    //�������
    GPIO_Init(GPIOA,&GPIO_Init_Struct);
    
    GPIO_Init_Struct.GPIO_Mode=GPIO_Mode_IN_FLOATING;                //RXDΪ��������
    GPIO_Init_Struct.GPIO_Pin=GPIO_Pin_10;                           //10��ΪRXD
    GPIO_Init(GPIOA,&GPIO_Init_Struct);                              
    
    USART_InitTypeDef Usart_Init_Struct;
    Usart_Init_Struct.USART_BaudRate=bound;                                      //������
    Usart_Init_Struct.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //��Ӳ��������
    Usart_Init_Struct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;                   //�շ�ģʽ
    Usart_Init_Struct.USART_Parity=USART_Parity_No;                             //��У��
    Usart_Init_Struct.USART_StopBits=USART_StopBits_1;                          //ֹͣλ1λ
    Usart_Init_Struct.USART_WordLength=USART_WordLength_8b;                     //����λ8λ
    USART_Init(USART1,&Usart_Init_Struct);
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);                                //���������ж�
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                             //NVIC�ж�����
    NVIC_InitTypeDef nitd;
    nitd.NVIC_IRQChannel=USART1_IRQn;
    nitd.NVIC_IRQChannelCmd=ENABLE;
    nitd.NVIC_IRQChannelPreemptionPriority=2;
    nitd.NVIC_IRQChannelSubPriority=2;
    NVIC_Init(&nitd);
    
    USART_Cmd(USART1,ENABLE);
}

/*******************************************************************************
** ��������: usart1_IRQHandler
** ��������: 
** ����˵��: None
** ����˵��: None
** ������Ա: temp_duan
** ��������: 2019-08-01
**------------------------------------------------------------------------------
** �޸���Ա:
** �޸�����:
** �޸�����:
**------------------------------------------------------------------------------
********************************************************************************/

uint8_t CO2_Data[6];                                        //��Ž��յ������ݰ�
 
void USART1_IRQHandler(void){
    static uint8_t index=0;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        CO2_Data[index]=USART_ReceiveData(USART1);
        if(index==0&&CO2_Data[index]!=0x2C){                //�̶����ݰ��Ŀ�ͷ
            return;
        }else{
            if(++index>=6) index=0;
        }
        USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);    //������ݽ��ձ�־λ
    }
}
