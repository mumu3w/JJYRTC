
#ifndef __IAP15W413AS_H__
#define __IAP15W413AS_H__

 #define IAP15W413AS_TEST

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
#define U1ENABLE()				do{ES=1;}while(0)			// 串口1使能
#define U1DISABLE()				do{ES=0;}while(0)

#define AUXR_T2R_ENABLE()		do{AUXR |= 0x10;}while(0)	// 使能定时器2作为串口1波特率发生器
#define AUXR_T2R_DISABLE()		do{AUXR &= 0xef;}while(0)

#define PINMxSET(x, y)			do{x |= (0x1 << (y));}while(0)
#define PINMxCLS(x, y)			do{x &= (~(0x1 << (y)));}while(0)

// config.h
#define	FOSC					6000000L					// 时钟频率
#define UART1_BAUD				9600L						// 波特率
//

// iap15w413as_config.h
void iap15w413as_config(void);
//

// uart.h
void uart1_init(void);
void send_data(u8 v);
void send_string(char *v);
void send_u8_decimal(u8 v);
//

// timer.h
extern xdata u16 timer_ms;
extern xdata u8 timer_flag;
extern xdata u16 timer_10ms;
extern xdata u8 bpc_flag;

void timer0_init(void);
void delay_ms(u16 ms);
void delay_s(u8 s);
//

// led.h
void led_config(void);
void led_red_enable(void);
void led_red_disable(void);
void led_color_enable(void);
void led_color_disable(void);

// time.h
#define	HOUR	    (60 * 60L)  /* # seconds in an hour */
#define	DAY	        (24 * HOUR) /* # seconds in a day */
#define	YEAR	    (365 * DAY) /* # seconds in a year */

typedef struct {
    int sec;
    int min;
    int hour;
    int day;
    int mon;
    int year;       // 实际年份(如:2019, 那么year=2019, year!=119)
    int week;       // 0 - 6
    long timestamp; // 1970.01.01 00:00:00 至今的秒数
} Date;

int leap_year(int year);
int day2date(Date *dat, int days);
void to_sec(Date *dat);
void to_date(Date *dat);
void to_week(Date *dat);
//

// bpc.h
void bpc_config(void);
void bpc_enable(void);
void bpc_disable(void);
int get_bpc(Date *dat);
//

// ds3231.h
void ds3231_config(void);
void ds3231_set_date(u8 y, u8 m, u8 d, u8 h, u8 mi, u8 s, u8 w);
void ds3231_get_date(u8 *y, u8 *m, u8 *d, u8 *h, u8 *mi, u8 *s, u8 *w);


#endif
