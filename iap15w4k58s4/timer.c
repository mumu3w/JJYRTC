
#include "iap15w4k58s4.h"

#define T0_1MS			(65536 - FOSC / 12 / 1000)

static u16 timing = 0;

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
}
#if 1
void timer4_init(void)	//1毫秒@6.000MHz
{
	T4T3M &= 0xDF;		//定时器时钟12T模式
	T4L = 0x0C;			//设置定时初值
	T4H = 0xFE;			//设置定时初值
	T4T3M |= 0x80;		//定时器4开始计时
	
	IE2 &= ~0x40;
}

u16 year1 = 0, mon1 = 0, day1 = 0, hour1 = 0, min1 = 0, sec1 = 0, week1 = 0;
void timer4_isr(void) interrupt 20
{
	static u16 i = 0;
	
	if (i >= 1000) i = 0;
	
	if (i == 50) ds1302_get_time(&year1, &mon1, &day1, &hour1, &min1, &sec1, &week1);
	if (i == 550) ds1302_get_time(&year1, &mon1, &day1, &hour1, &min1, &sec1, &week1);
	
	i++;
}
#endif
