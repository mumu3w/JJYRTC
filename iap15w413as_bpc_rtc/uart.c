
#include "iap15w413as.h"

static xdata u8 busy;

void uart1_init(void)	// 9600bps@6.000MHz
{
	SCON = 0x50;		// 8位数据,可变波特率
	AUXR |= 0x01;		// 串口1选择定时器2为波特率发生器
	AUXR &= 0xFB;		// 定时器2时钟为Fosc/12,即12T
	T2L = (65536 - (FOSC / (12 * 4 * UART1_BAUD)));
	T2H = (65536 - (FOSC / (12 * 4 * UART1_BAUD))) >> 8;
	
	busy = 0;
	TI = 1;
	AUXR_T2R_ENABLE();	// 启动定时器2
}

void send_data(u8 v)
{
	while (busy);
	busy = 1;
	SBUF = v;
}

void send_string(char *v)
{
	while (*v) send_data((u8)*v++);
}

void send_u8_decimal(u8 v)
{
	u8 tmp[4] = "000";
	
	tmp[2] = v % 10;
	tmp[1] = (v - v / 100 * 100) / 10;
	tmp[0] = (v - tmp[2] - tmp[1] * 10) / 100;
	
	tmp[2] += '0';
	tmp[1] += '0';
	tmp[0] += '0';
	
	send_string(tmp);
}

void uart1_isr(void) interrupt 4
{
	if (RI) {RI = 0;}
	if (TI) {TI = 0; busy = 0;}
}
