#include"iostm8s105k4.h"
unsigned char HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void Init_AD(void)
{
		ADC_CSR=0x00; //选择通道
		ADC_CR2|=0x08; // 数据排列,右对齐
		ADC_TDRL=0x01;
}
void Init_UART2(void)
{
      UART2_CR1=0x00;
      UART2_CR2=0x00;
      UART2_CR3=0x00;
      // 设置波特率，必须注意以下几点：
      // (1) 必须先写BRR2
      // (2) BRR1存放的是分频系数的第11位到第4位，
      // (3) BRR2存放的是分频系数的第15位到第12位，和第3位
      // 到第0位
      // 例如对于波特率位9600时，分频系数=2000000/9600=208
      // 对应的十六进制数为00D0，BBR1=0D,BBR2=00

      UART2_BRR2=0x00;
      UART2_BRR1=0x0d;

      UART2_CR2=0x2c;//允许接收，发送，开接收中断
}
void UART2_sendchar(unsigned char c)
{
      while((UART2_SR & 0x80)==0x00);
      UART2_DR=c;
}
void UART2_sendhex(unsigned char dat)
{
		UART2_sendchar('0');
		
                UART2_sendchar('x');
		
                UART2_sendchar(HexTable[dat>>4]);
		
                UART2_sendchar(HexTable[dat&0x0f]);
		
                UART2_sendchar(' ');
}
void UART2_sendstr(unsigned char *dat)
{
		while(*dat!='\0')
		
                {
				
                  UART2_sendchar(*dat);
				
                  dat++;
				//delay2us();
		
                }
}

main()
{
		int i=0;
		Init_UART2();
		Init_AD();
		while (1)
		{
				ADC_CR1|=0x01;
				for(i=0;i<100;i++); // 延时一段时间，至少7uS，保证ADC 模块上电王城
				ADC_CR1|=0x01;      // 再次将CR1寄存器的最低位置1,开启AD转换

				while((ADC_CSR & 0x80)!=0x80); // 等待转换结束
				//UART2_sendchar(ADC_DRH);
				//UART2_sendchar(ADC_DRL);
				UART2_sendhex(ADC_DRH);
				UART2_sendhex(ADC_DRL);
				UART2_sendstr("\r\n");
				ADC_CSR&=(~0x80);
		}
}