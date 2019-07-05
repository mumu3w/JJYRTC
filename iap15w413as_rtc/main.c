
#include "iap15w413as.h"

void print_time(Date *d)
{
	send_u8_decimal(d->year);
	send_string("/");
	send_u8_decimal(d->mon);
	send_string("/");
	send_u8_decimal(d->day);
	send_string("/");
	
	send_u8_decimal(d->hour);
	send_string("/");
	send_u8_decimal(d->min);
	send_string("/");
	send_u8_decimal(d->sec);
	send_string("/");
	
	send_u8_decimal(d->week);
	send_string(" \r\n");
}

void main(void)
{
	//u8 i = 0;
	u8 y = 19, m = 6, d = 25;
	u8 h = 20, mi= 6, s = 00, ss;
	u8 w = 2;
	Date date;
	u8 oldsec = 60;
	u8 update_flag = 1;
	
	iap15w413as_config();

	send_string("IAP15W413AS DS3231SN Test. \r\n");
	
	//ds3231_set_date(y, m, d, h, mi, s, w);

	while (1) {
		delay_ms(200);
		ds3231_get_date(&y, &m, &d, &h, &mi, &s, &w);
		if (oldsec != s) {
			date.year = y;
			date.mon = m;
			date.day = d;
			date.hour = h;
			date.min = mi;
			date.sec = s;
			date.week = w;
			print_time(&date);
			
			ds1302_get_date(&y, &m, &d, &h, &mi, &ss, &w);
			date.year = y;
			date.mon = m;
			date.day = d;
			date.hour = h;
			date.min = mi;
			date.sec = ss;
			date.week = w;
			print_time(&date);
			
			oldsec = s;
		}
		
		if (update_flag) {
			if (update_time() == OK)
				update_flag = 0;
		}
		//send_u8_decimal(i);
		//send_string(" \r\n");
		//i++;
	}
}
