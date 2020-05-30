// Header:
// File Name: main.c
// Author: elvin
// Date: 2020-05-18

#include "stm32f10x.h"
#include "my_usart2.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "usart3.h"
#include "rs485.h"
#include "ds18b20.h"

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{		
	short temperature; 
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	//USART2_Init(9600);
	uart_init(115200);	 //串口初始化为115200
	usart3_init(115200);  
	TIM3_Int_Init(29999,7199); 		//3S中断一次 Tout= ((arr+1)*(psc+1))/Tclk；
 	TIM2_Cap_Init();	//定时器2计数脉冲
	
	while(DS18B20_Init())	//DS18B20初始化	
	{
		printf("DS18B20 Error\r\n");//打印次数
 		delay_ms(200);
	}	
	
	while(1)
	{
		
		delay_ms(1000);
	    temperature=DS18B20_Get_Temp();
	    //temperature=-350;
		printf("CNT:%d 次\r\n",CNT);//打印次数
		printf("频率:%d Hz\r\n",fq);//打印频率
		printf("转速:%d r/min\r\n",rate);//打印转速
		printf("温度:%d  C\r\n",temperature);//打印温度 + -25.0摄氏度表示为：+ -250摄氏度
		printf("温度十六进制:%02x  。C\r\n",temperature);//打印温度 + -25.0摄氏度表示为：+ -250摄氏度	
//		printf("温度:%d  C\r\n",(u16)temperature);//打印温度 + -25.0摄氏度表示为：+ -250摄氏度
//		printf("温度十六进制:%02x  C\r\n",(u16)temperature);//打印温度 + -25.0摄氏度表示为：+ -250摄氏度
		printf("\r\n");//换行

		RS485_RX_Service();
	}
	
}
