
#include "iap15w413as.h"

#define T0_1MS			(65536 - FOSC / 12 / 1000)

static xdata u16 timing = 0;
xdata u16 timer_ms = 0;
xdata u8 timer_flag = 0;

xdata u16 timer_10ms = 0;
xdata u8 bpc_flag = 0;

void timer0_init(void)	// 1毫秒@6.000MHz
{
	AUXR &= 0x7F;		// 定时器时钟12T模式
	TMOD &= 0xF0;		// 设置定时器模式
	TL0 = T0_1MS;		// 设置定时初值
	TH0 = T0_1MS >> 8;	// 设置定时初值
	TF0 = 0;			// 清除TF0标志
	TR0 = 1;			// 定时器0开始计时
}

void delay_ms(u16 ms)
{
	timing = ms;
	while (timing != 0x0);
}

void delay_s(u8 s)
{
	u8 i;
	
	for (i = 0; i < s; i++) delay_ms(1000);
}
	
void timer0_isr(void) interrupt 1
{
	if (timing != 0x0) timing--;
	if (timer_ms == 1000) {timer_ms = 0; timer_flag = 1;}
	timer_ms++;
	if (timer_10ms == 60000) {timer_10ms = 0;}
	if (timer_10ms % 10 == 0) {bpc_flag = 1;}
	timer_10ms++;
}
