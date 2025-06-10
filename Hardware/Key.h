#ifndef __KEY_H
#define __KEY_H

void Key_Init(void);
uint8_t Key_GetNum(void);

#define KEY1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)

#endif
