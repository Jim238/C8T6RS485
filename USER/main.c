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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//USART2_Init(9600);
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	usart3_init(115200);  
	TIM3_Int_Init(29999,7199); 		//3S�ж�һ�� Tout= ((arr+1)*(psc+1))/Tclk��
 	TIM2_Cap_Init();	//��ʱ��2��������
	
	while(DS18B20_Init())	//DS18B20��ʼ��	
	{
		printf("DS18B20 Error\r\n");//��ӡ����
 		delay_ms(200);
	}	
	
	while(1)
	{
		
		delay_ms(1000);
	    temperature=DS18B20_Get_Temp();
	    //temperature=-350;
		printf("CNT:%d ��\r\n",CNT);//��ӡ����
		printf("Ƶ��:%d Hz\r\n",fq);//��ӡƵ��
		printf("ת��:%d r/min\r\n",rate);//��ӡת��
		printf("�¶�:%d  C\r\n",temperature);//��ӡ�¶� + -25.0���϶ȱ�ʾΪ��+ -250���϶�
		printf("�¶�ʮ������:%02x  ��C\r\n",temperature);//��ӡ�¶� + -25.0���϶ȱ�ʾΪ��+ -250���϶�	
//		printf("�¶�:%d  C\r\n",(u16)temperature);//��ӡ�¶� + -25.0���϶ȱ�ʾΪ��+ -250���϶�
//		printf("�¶�ʮ������:%02x  C\r\n",(u16)temperature);//��ӡ�¶� + -25.0���϶ȱ�ʾΪ��+ -250���϶�
		printf("\r\n");//����

		RS485_RX_Service();
	}
	
}
