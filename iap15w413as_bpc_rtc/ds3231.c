
#include "iap15w413as.h"

#define DS3231_WADDR	0xd0
#define DS3231_RADDR	0xd1

sbit i2c_sda = P1^0;
sbit i2c_scl = P1^1;

static void i2c_delay(void)	// 5us @6.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 3;
	while (--i);
}

static void i2c_start(void)
{
	i2c_sda = 1;
	i2c_scl = 1;
	i2c_delay();
	i2c_sda = 0;
	i2c_delay();
	i2c_scl = 0;
}

static void i2c_stop(void)
{
	i2c_sda = 0;
	i2c_scl = 1;
	i2c_delay();
	i2c_sda = 1;
}

static void i2c_write_byte(u8 Byte)
{
	u8 i;

	for (i = 0; i < 8; i++) {
		if (Byte & 0x80) i2c_sda = 1;
		else i2c_sda = 0;

		i2c_delay();
		i2c_scl = 1;
		i2c_delay();
		i2c_scl = 0;

		if (i == 7) i2c_sda = 1;
		Byte <<= 0x1;
		i2c_delay();
	}
}

static u8 i2c_read_byte(void)
{
	u8 i, v = 0;

	for (i = 0; i < 8; i++) {
		v <<= 0x1;
		i2c_scl = 1;
		i2c_delay();

		if (i2c_sda == 1) v++;
		i2c_scl = 0;
		i2c_delay();
	}

	return v;
}

static u8 i2c_wait_ack(void)
{
	u8 re;

	i2c_sda = 1;
	i2c_delay();
	i2c_scl = 1;
	i2c_delay();

	if (i2c_sda == 1) re = 1;
	else re = 0;

	i2c_scl = 0;
	i2c_delay();

	return re;
}

static u8 ds3231_bcd2hex(u8 bcd)
{
	return (bcd >> 4) * 10 + (bcd & 0xf);
}

static u8 ds3231_hex2bcd(u8 hex)
{
	return (hex / 10 << 4) + (hex % 10);
}

static void ds3231_write_byte(u8 addr, u8 v)
{
	i2c_start();
	i2c_write_byte(DS3231_WADDR);
	i2c_wait_ack();
	i2c_write_byte(addr);
	i2c_wait_ack();
	i2c_write_byte(v);
	i2c_wait_ack();
	i2c_stop();
}

static ds3231_read_byte(u8 addr)
{
	u8 v;

	i2c_start();
	i2c_write_byte(DS3231_WADDR);
	i2c_wait_ack();
	i2c_write_byte(addr);
	i2c_wait_ack();

	i2c_start();
	i2c_write_byte(DS3231_RADDR);
	i2c_wait_ack();
	v = i2c_read_byte();
	i2c_stop();

	return v;
}

void ds3231_config(void)
{
	u16 i;

	ds3231_write_byte(0x0e, 0x0);
	//delay_ms(500);
	for (i = 0; i < 200; i++) i2c_delay();
	ds3231_write_byte(0x0f, 0x0);
	//delay_ms(500);
	for (i = 0; i < 200; i++) i2c_delay();
}

void ds3231_set_date(u8 y, u8 m, u8 d, u8 h, u8 mi, u8 s, u8 w)
{
	ds3231_write_byte(0x06, ds3231_hex2bcd(y));
	ds3231_write_byte(0x05, ds3231_hex2bcd(m));
	ds3231_write_byte(0x04, ds3231_hex2bcd(d));
	ds3231_write_byte(0x02, ds3231_hex2bcd(h));
	ds3231_write_byte(0x01, ds3231_hex2bcd(mi));
	ds3231_write_byte(0x00, ds3231_hex2bcd(s));
	ds3231_write_byte(0x03, ds3231_hex2bcd(w));
}

void ds3231_get_date(u8 *y, u8 *m, u8 *d, u8 *h, u8 *mi, u8 *s, u8 *w)
{
//	u8 v;
	
	*y = ds3231_bcd2hex(ds3231_read_byte(0x06));
	*m = ds3231_bcd2hex(ds3231_read_byte(0x05));
	*d = ds3231_bcd2hex(ds3231_read_byte(0x04));

	*h = ds3231_bcd2hex(ds3231_read_byte(0x02) & 0x3f);
	*mi = ds3231_bcd2hex(ds3231_read_byte(0x01));
	*s = ds3231_bcd2hex(ds3231_read_byte(0x00));

	*w = ds3231_bcd2hex(ds3231_read_byte(0x03));

	//v = ds3231_read_byte(0x0e);
	//v |= 0x20;
	//ds3231_write_byte(0x0e, v);
	//*t = ds3231_bcd2hex(ds3231_read_byte(0x011));
}
