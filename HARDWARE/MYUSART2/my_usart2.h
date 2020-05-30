#ifndef __MY_USART2_H
#define __MY_USART2_H

#include "stm32f10x.h"

#define BUF_SIZE 256

extern u16 USART2_RX_STA;
extern u8 USART2_RX_BUF[BUF_SIZE];
extern u32 BUF_COUNT;
extern u8 BUF_CHECK;

void USART2_Init(u32 bound);
void USART2_SendChar(char c);
void USART2_SendString(char *s);
void USART2_SendArray(char array[], int num);

#endif
