#include "rs485.h"
#include "crc16.h" 
#include "usart3.h"
#include "timer.h"
#include "ds18b20.h"
#include "usart.h"

/////////////////////////////////////////////////////////////////////////////////////
//主机命令区
u8   SlaverAddr=0x01;   //从机地址1
u8   SlaverAddr2=0x02;   //从机地址2

//H：高位  L：低位
//次数 
char	Tx_CAN_H = 0x00;  
char	Tx_CAN_L = 0x00;
//频率
char	Tx_Fq_H = 0x00;
char	Tx_Fq_L = 0x00;
//转速
char	Tx_Rate_H = 0x00;
char	Tx_Rate_L= 0x00;
//温度 + -25.0摄氏度表示为：+ -250摄氏度
char	Tx_Temperature_H = 0x00;
char	Tx_Temperature_L= 0x00;


/////////////////////////////////////////////////////////////////////////////////////
//RS485服务程序，用于处理接收到的数据(请在主函数中循环调用)
void RS485_RX_Service(void)
{
	u16 len;
	u16 calCRC;
	u16 recCRC;
	if(USART3_RX_STA&0x8000) //接收到数据
	{
		len = USART3_RX_STA&0x3fff;//得到此次接收到的数据长度
		if(USART3_RX_BUF[0]== SlaverAddr || USART3_RX_BUF[0]== SlaverAddr2)//地址正确
		{
			if(USART3_RX_BUF[1]==04 || USART3_RX_BUF[1]==03 || USART3_RX_BUF[1]==06)//功能码正确
			  {
				calCRC = CRC_Compute(USART3_RX_BUF, len-2);//计算所接收数据的CRC
				recCRC = USART3_RX_BUF[len-1] | (((u16)USART3_RX_BUF[len-2])<<8);//接收到的CRC(低字节在前，高字节在后)
				if(calCRC==recCRC)//CRC校验正确
				{
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					switch(USART3_RX_BUF[1])//根据不同的功能码进行处理
					{    
						
						case 04: 
						{
							//Modbus_04_Solve();															
							break;
						}
						case 03:													
						{
						   Modbus_03_Solve(); //自己编写需要发送的数据					
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
		USART3_RX_STA=0; //清零，重新接收
    }                 
}

void Modbus_03_Solve(void)
{
	u16 calCRC;
	short temperature; 
	
	USART3_TX_BUF[0] = 0X01;  //地址
	USART3_TX_BUF[1] = 0X03;  //功能码
	USART3_TX_BUF[2] = 0X08;  //长度
	
    //次数
	Tx_CAN_H=(((short)CNT)&0XFF00) >>8;
	Tx_CAN_L=((short)CNT)&0X00FF;
	USART3_TX_BUF[3] = Tx_CAN_H;
	USART3_TX_BUF[4] = Tx_CAN_L;
	 
	//频率
	Tx_Fq_H=(((short)fq)&0XFF00) >>8;
	Tx_Fq_L= ((short)fq)&0X00FF;
	USART3_TX_BUF[5] = Tx_Fq_H;
	USART3_TX_BUF[6] = Tx_Fq_L;
	
	 //转速
	Tx_Rate_H= (((short)rate)&0XFF00) >>8;
	Tx_Rate_L= ((short)rate)&0X00FF;
	USART3_TX_BUF[7] = Tx_Rate_H;
	USART3_TX_BUF[8] = Tx_Rate_L;
	
	//温度 + -25.0摄氏度表示为：+ -250摄氏度
	temperature=DS18B20_Get_Temp();
	Tx_Temperature_H= (temperature&0XFF00)>>8;
	Tx_Temperature_L= (temperature)&0X00FF;
	USART3_TX_BUF[9] = Tx_Temperature_H;
	USART3_TX_BUF[10] = Tx_Temperature_L;


	
	calCRC=CRC_Compute(USART3_TX_BUF,11);
    USART3_TX_BUF[11]=(calCRC>>8)&0xFF;//CRC高字节
    USART3_TX_BUF[12]=(calCRC)&0xFF;//CRC低字节
	
	USART3_SendArray(USART3_TX_BUF , 13);
	
	
}


