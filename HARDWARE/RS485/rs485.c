#include "rs485.h"
#include "crc16.h" 
#include "usart3.h"
#include "timer.h"
#include "ds18b20.h"
#include "usart.h"

/////////////////////////////////////////////////////////////////////////////////////
//����������
u8   SlaverAddr=0x01;   //�ӻ���ַ1
u8   SlaverAddr2=0x02;   //�ӻ���ַ2

//H����λ  L����λ
//���� 
char	Tx_CAN_H = 0x00;  
char	Tx_CAN_L = 0x00;
//Ƶ��
char	Tx_Fq_H = 0x00;
char	Tx_Fq_L = 0x00;
//ת��
char	Tx_Rate_H = 0x00;
char	Tx_Rate_L= 0x00;
//�¶� + -25.0���϶ȱ�ʾΪ��+ -250���϶�
char	Tx_Temperature_H = 0x00;
char	Tx_Temperature_L= 0x00;


/////////////////////////////////////////////////////////////////////////////////////
//RS485����������ڴ�����յ�������(������������ѭ������)
void RS485_RX_Service(void)
{
	u16 len;
	u16 calCRC;
	u16 recCRC;
	if(USART3_RX_STA&0x8000) //���յ�����
	{
		len = USART3_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
		if(USART3_RX_BUF[0]== SlaverAddr || USART3_RX_BUF[0]== SlaverAddr2)//��ַ��ȷ
		{
			if(USART3_RX_BUF[1]==04 || USART3_RX_BUF[1]==03 || USART3_RX_BUF[1]==06)//��������ȷ
			  {
				calCRC = CRC_Compute(USART3_RX_BUF, len-2);//�������������ݵ�CRC
				recCRC = USART3_RX_BUF[len-1] | (((u16)USART3_RX_BUF[len-2])<<8);//���յ���CRC(���ֽ���ǰ�����ֽ��ں�)
				if(calCRC==recCRC)//CRCУ����ȷ
				{
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					switch(USART3_RX_BUF[1])//���ݲ�ͬ�Ĺ�������д���
					{    
						
						case 04: 
						{
							//Modbus_04_Solve();															
							break;
						}
						case 03:													
						{
						   Modbus_03_Solve(); //�Լ���д��Ҫ���͵�����					
							break;
						}
						case 06:																									
						{
							//Modbus_06_Solve();															
							break;
						}
															
					}
				}                                                                                       
			}
		}                              
		USART3_RX_STA=0; //���㣬���½���
    }                 
}

void Modbus_03_Solve(void)
{
	u16 calCRC;
	short temperature; 
	
	USART3_TX_BUF[0] = 0X01;  //��ַ
	USART3_TX_BUF[1] = 0X03;  //������
	USART3_TX_BUF[2] = 0X08;  //����
	
    //����
	Tx_CAN_H=(((short)CNT)&0XFF00) >>8;
	Tx_CAN_L=((short)CNT)&0X00FF;
	USART3_TX_BUF[3] = Tx_CAN_H;
	USART3_TX_BUF[4] = Tx_CAN_L;
	 
	//Ƶ��
	Tx_Fq_H=(((short)fq)&0XFF00) >>8;
	Tx_Fq_L= ((short)fq)&0X00FF;
	USART3_TX_BUF[5] = Tx_Fq_H;
	USART3_TX_BUF[6] = Tx_Fq_L;
	
	 //ת��
	Tx_Rate_H= (((short)rate)&0XFF00) >>8;
	Tx_Rate_L= ((short)rate)&0X00FF;
	USART3_TX_BUF[7] = Tx_Rate_H;
	USART3_TX_BUF[8] = Tx_Rate_L;
	
	//�¶� + -25.0���϶ȱ�ʾΪ��+ -250���϶�
	temperature=DS18B20_Get_Temp();
	Tx_Temperature_H= (temperature&0XFF00)>>8;
	Tx_Temperature_L= (temperature)&0X00FF;
	USART3_TX_BUF[9] = Tx_Temperature_H;
	USART3_TX_BUF[10] = Tx_Temperature_L;


	
	calCRC=CRC_Compute(USART3_TX_BUF,11);
    USART3_TX_BUF[11]=(calCRC>>8)&0xFF;//CRC���ֽ�
    USART3_TX_BUF[12]=(calCRC)&0xFF;//CRC���ֽ�
	
	USART3_SendArray(USART3_TX_BUF , 13);
	
	
}


