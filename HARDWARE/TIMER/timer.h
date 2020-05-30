#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern u32  CNT;		//次数    				
extern u32	fq;	//频率
extern u32 rate;  //转速

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_Cap_Init(void);
void TIM4_Int_Init(u16 arr,u16 psc);
#endif
