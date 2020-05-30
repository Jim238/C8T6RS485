#include "my_usart2.h"

/*����2��������״̬λ, 0Ϊû�н��յ�����, 1Ϊ����������*/
u16 USART2_RX_STA = 0;

u8 USART2_RX_BUF[BUF_SIZE];
u32 BUF_COUNT = 0;
uint8_t Uart2_head1;              //֡ͷ1
uint8_t Uart2_head2;              //֡ͷ2
int Uart2_checkSum;              //��ͼ��鷵��ֵ

void USART2_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /*ʹ�ܴ���2����ʱ��*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /*��λ����2*/
    USART_DeInit(USART2);

    /*TX���Ŷ���*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*RX���Ŷ���*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*����2��ʼ��*/
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    /*����2�жϳ�ʼ��*/
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*����2�����ж�ʹ��*/
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    /*ʹ�ܴ���2*/
    USART_Cmd(USART2, ENABLE);

    /*���������ɱ�־*/
    USART_ClearFlag(USART2, USART_FLAG_TC);
}


int checkNum(const u8 *buf, int len){
	  int sum = 0;
		int i = 0;
		for (i = BUF_COUNT  - len ; i < BUF_COUNT -1; i++) {
		sum = sum + buf[i];
	}
		return sum;
}

void USART2_IRQHandler() {
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		USART2_RX_BUF[BUF_COUNT] = USART_ReceiveData(USART2);
		BUF_COUNT++;
    BUF_COUNT &= 0xFF;
	}
	
	if(USART2_RX_BUF[BUF_COUNT-1] == 0x00)   //֡ͷ0
	{
		Uart2_head1 = BUF_COUNT-1;   // ==0
	}else if((BUF_COUNT-1 == Uart2_head1+1) && (USART2_RX_BUF[BUF_COUNT-1] == 0x0A) ) //֡ͷ1
   { 
		 Uart2_head2 = BUF_COUNT-1;   //==1
	 }else if(BUF_COUNT-1 == 18) //У��֡
    {
			Uart2_checkSum = checkNum( USART2_RX_BUF, 19);  //
      if( USART2_RX_BUF[BUF_COUNT-1] == (Uart2_checkSum & 0xFF) ) //
      {
			USART2_RX_STA = 1;
      }
    }
}

void USART2_SendChar(char c) {
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET); //�ж������Ƿ�д��Ĵ���
	USART_SendData(USART2, c);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET); //�ȴ��������
}

void USART2_SendString(char *s) {
	while(*s) {
		USART2_SendChar(*s++);
	}
}

void USART2_SendArray(char array[], int num) {
	for(int i=0; i<num; i++) {
		USART2_SendChar(array[i]);
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET); //�ȴ��������
	}
}
