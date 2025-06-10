/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, temp_duan, China, DY.
**                           All Rights Reserved
**
**                           By(德阳科派科技有限公司)
**                             http://www.kpiot.top
**
**----------------------------------文件信息------------------------------------
** 文件名称: usart1.c
** 创建人员: temp_duan
** 创建日期: 2019-08-01
** 文档描述: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "usart.h"
/*******************************************************************************
** 函数名称: usart1_init
** 功能描述: 
** 参数说明: bound: [输入/出] 
** 返回说明: None
** 创建人员: temp_duan
** 创建日期: 2019-08-01
**------------------------------------------------------------------------------
** 修改人员:
** 修改日期:
** 修改描述:
**------------------------------------------------------------------------------
********************************************************************************/
void usart1_init(u32 bound)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能外设时钟
    
    GPIO_InitTypeDef GPIO_Init_Struct;
    GPIO_Init_Struct.GPIO_Mode=GPIO_Mode_AF_PP;                      //TXD为复用推挽输出
    GPIO_Init_Struct.GPIO_Pin=GPIO_Pin_9;                            //9号为TXD
    GPIO_Init_Struct.GPIO_Speed=GPIO_Speed_50MHz;                    //这个随意
    GPIO_Init(GPIOA,&GPIO_Init_Struct);
    
    GPIO_Init_Struct.GPIO_Mode=GPIO_Mode_IN_FLOATING;                //RXD为浮空输入
    GPIO_Init_Struct.GPIO_Pin=GPIO_Pin_10;                           //10号为RXD
    GPIO_Init(GPIOA,&GPIO_Init_Struct);                              
    
    USART_InitTypeDef Usart_Init_Struct;
    Usart_Init_Struct.USART_BaudRate=bound;                                      //波特率
    Usart_Init_Struct.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //无硬件控制流
    Usart_Init_Struct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;                   //收发模式
    Usart_Init_Struct.USART_Parity=USART_Parity_No;                             //无校验
    Usart_Init_Struct.USART_StopBits=USART_StopBits_1;                          //停止位1位
    Usart_Init_Struct.USART_WordLength=USART_WordLength_8b;                     //数据位8位
    USART_Init(USART1,&Usart_Init_Struct);
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);                                //开启接收中断
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                             //NVIC中断配置
    NVIC_InitTypeDef nitd;
    nitd.NVIC_IRQChannel=USART1_IRQn;
    nitd.NVIC_IRQChannelCmd=ENABLE;
    nitd.NVIC_IRQChannelPreemptionPriority=2;
    nitd.NVIC_IRQChannelSubPriority=2;
    NVIC_Init(&nitd);
    
    USART_Cmd(USART1,ENABLE);
}

/*******************************************************************************
** 函数名称: usart1_IRQHandler
** 功能描述: 
** 参数说明: None
** 返回说明: None
** 创建人员: temp_duan
** 创建日期: 2019-08-01
**------------------------------------------------------------------------------
** 修改人员:
** 修改日期:
** 修改描述:
**------------------------------------------------------------------------------
********************************************************************************/

uint8_t CO2_Data[6];                                        //存放接收到的数据包
 
void USART1_IRQHandler(void){
    static uint8_t index=0;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        CO2_Data[index]=USART_ReceiveData(USART1);
        if(index==0&&CO2_Data[index]!=0x2C){                //固定数据包的开头
            return;
        }else{
            if(++index>=6) index=0;
        }
        USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);    //清除数据接收标志位
    }
}
