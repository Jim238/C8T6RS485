#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern u32  CNT;		//����    				
extern u32	fq;	//Ƶ��
extern u32 rate;  //ת��

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_Cap_Init(void);
void TIM4_Int_Init(u16 arr,u16 psc);
#endif
