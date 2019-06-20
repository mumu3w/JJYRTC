
#include "iap15w4k58s4.h"

#define R_YEAR	0x8d
#define W_YEAR	0x8c

#define R_MON	0x89
#define W_MON	0x88

#define R_DAY	0x87
#define W_DAY	0x86

#define R_HOUR	0x85
#define W_HOUR	0x84

#define R_MIN	0x83
#define W_MIN	0x82

#define R_SEC	0x81
#define W_SEC	0x80

#define R_WEEK	0x8b
#define W_WEEK	0x8a

sbit DSIO = DS1302_DSIO;
sbit RST = DS1302_RST;
sbit SCLK = DS1302_SCLK;

static void ds1302_write(u8 addr, u8 v);
static u8 ds1302_read(u8 addr);

void ds1302_init(void)
{
	PINMxCLS(P1M1, 3);
	PINMxCLS(P1M0, 3);
	PINMxCLS(P1M1, 4);
	PINMxCLS(P1M0, 4);
	PINMxCLS(P1M1, 5);
	PINMxCLS(P1M0, 5);
	
	ds1302_write(0x8E,0X00);	//关闭写保护功能
	// ds1302_write(0x8E,0x80);	//打开写保护功能
}

static void ds1302_write(u8 addr, u8 v)
{
	u8 i;
	
	RST = 0;
	_nop_();
	SCLK = 0;
	_nop_();
	RST = 1;
	_nop_();
	
	for (i = 0; i < 8; i++) {
		DSIO = addr & 0x1;
		addr >>= 1;
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();
	}
	
	for (i = 0; i < 8; i++) {
		DSIO = v & 0x1;
		v >>= 1;
		SCLK = 1;
		_nop_();
		SCLK = 0;
		_nop_();
	}
	
	RST = 0;
	_nop_();
}

static u8 ds1302_read(u8 addr)
{
	u8 i, v, v1;
	
	RST = 0;
	_nop_();
	SCLK = 0;	//先将SCLK置低电平。
	_nop_();
	RST = 1;	//然后将RST(CE)置高电平。
	_nop_();

	for(i = 0; i < 8; i++) {	//开始传送八位地址命令
		DSIO = addr & 0x01;		//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;				//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;				//DS1302下降沿时，放置数据
		_nop_();
	}
	
	_nop_();
	for(i = 0; i < 8; i++) {	//读取8位数据
		v1 = DSIO;				//从最低位开始接收
		v = (v >> 1) | (v1 << 7);
		SCLK = 1;
		_nop_();
		SCLK = 0;				//DS1302下降沿时，放置数据
		_nop_();
	}

	RST = 0;
	_nop_();					//以下为DS1302复位的稳定时间,必须的。
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	return v;
}

u8 hex2bcd(u8 hex)
{
	return (hex / 10 << 4) + (hex % 10);
}

u8 bcd2hex(u8 bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0xf);
}

void ds1302_get_time(
	u16 *y, u16 *mo, u16 *d, 
	u16 *h, u16 *mi, u16 *s,
	u16 *w)
{
	*y = ds1302_read(R_YEAR);
	*y = bcd2hex(*y);
	
	*mo = ds1302_read(R_MON);
	*mo = bcd2hex(*mo);
	
	*d = ds1302_read(R_DAY);
	*d = bcd2hex(*d);
	
	*h = ds1302_read(R_HOUR);
	*h = bcd2hex(*h);
	
	*mi = ds1302_read(R_MIN);
	*mi = bcd2hex(*mi);
	
	*s = ds1302_read(R_SEC);
	*s = bcd2hex(*s);
	
	*w = ds1302_read(R_WEEK);
	*w = bcd2hex(*w);
}

void ds1302_set_time(
	u8 y, u8 mo, u8 d, 
	u8 h, u8 mi, u8 s,
	u8 w)
{
	y = hex2bcd(y);
	ds1302_write(W_YEAR, y);
	
	mo = hex2bcd(mo);
	ds1302_write(W_MON, mo);
	
	d = hex2bcd(d);
	ds1302_write(W_DAY, d);
	
	h = hex2bcd(h);
	ds1302_write(W_HOUR, h);
	
	mi = hex2bcd(mi);
	ds1302_write(W_MIN, mi);
	
	s = hex2bcd(s);
	ds1302_write(W_SEC, s);
	
	w = hex2bcd(w);
	ds1302_write(W_WEEK, w);
}

void send_time(void)
{
	u16 year = 0, mon = 0, day = 0, hour = 0, min = 0, sec = 0, week = 0;
	
	ds1302_get_time(&year, &mon, &day, &hour, &min, &sec, &week);
	send_data(hex2bcd(year));
	send_data(hex2bcd(mon));
	send_data(hex2bcd(day));
	send_data(hex2bcd(hour));
	send_data(hex2bcd(min));
	send_data(hex2bcd(sec));
	send_data(hex2bcd(week));
}
