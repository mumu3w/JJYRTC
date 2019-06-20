
#ifndef __IAP15W4K58S4_H__
#define __IAP15W4K58S4_H__

	#define JJYDEBUG
	
#include <stc/stc15.h>
#include <intrins.h>

typedef unsigned char			u8;
typedef unsigned int			u16;
typedef unsigned long			u32;

#define HIGH					1
#define LOWER					0

#define OK      				0
#define ERR     				-1
			
#define TRUE    				(1==1)
#define FALSE   				(0==1)

#define ISRENABLE()				do{EA=1;}while(0)			// 总中断使能
#define ISRDISABLE()			do{EA=0;}while(0)
#define T0ENABLE()				do{ET0=1;}while(0)			// 定时器0使能
#define T0DISABLE()				do{ET0=0;}while(0)
#define T4ENABLE()				do{IE2 |= 0x40;}while(0)	// 定时器4使能
#define T4DISABLE()				do{IE2 &= ~0x40;}while(0)
#define U1ENABLE()				do{ES=1;}while(0)			// 串口1使能
#define U1DISABLE()				do{ES=0;}while(0)

#define AUXR_T2R_ENABLE()		do{AUXR |= 0x10;}while(0)	// 使能定时器2作为串口1波特率发生器
#define AUXR_T2R_DISABLE()		do{AUXR &= 0xef;}while(0)

#define PINMxSET(x, y)			do{x |= (0x1 << (y));}while(0)
#define PINMxCLS(x, y)			do{x &= (~(0x1 << (y)));}while(0)

// config.h
#define	FOSC					6000000L					// 时钟频率
#define UART1_BAUD				9600						// 波特率
//

// iap15w4k58s4_config.h
void iap15w4k58s4_config(void);
//

// uart.h
#define JJY_TIME_MESS_SIZE		33

extern xdata u8 jjy_time_mess[JJY_TIME_MESS_SIZE + 1];
extern xdata u8 JjyTimeMessFlag;

void uart1_init(void);
void send_data(u8 v);
void send_string(char *v);
void send_u8_decimal(u8 v);
//

// timer.h
extern u16 year1, mon1, day1, hour1, min1, sec1, week1;

void timer4_init(void);
void timer0_init(void);
void delay_ms(u16 ms);
void delay_s(u8 s);
// 

// update_time.h
int update_time(void);
//

// ds1302.h
#define DS1302_DSIO				P1^4		// IO
#define DS1302_RST				P1^5		// CE
#define DS1302_SCLK 			P1^3		// SCLK

#define PINMxSET(x, y)			do{x |= (0x1 << (y));}while(0)
#define PINMxCLS(x, y)			do{x &= (~(0x1 << (y)));}while(0)

void ds1302_init(void);
void ds1302_get_time(u16 *y, u16 *mo, u16 *d, u16 *h, u16 *mi, u16 *s, u16 *w);
void ds1302_set_time(u8 y, u8 mo, u8 d, u8 h, u8 mi, u8 s, u8 w);
void send_time(void);
u8 hex2bcd(u8 hex);
u8 bcd2hex(u8 bcd);
// 

// lcd1602.h
void lcd1602_write_com(u8 com);
void lcd1602_write_data(u8 dat);
void lcd1602_init();
void lcd1602_pos(u8 x, u8 y);
void lcd1602_disp(u8 *s, u8 len);
//

#endif
