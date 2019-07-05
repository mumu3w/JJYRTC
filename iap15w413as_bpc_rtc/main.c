
#include "iap15w413as.h"

void print_time(Date *dat)
{
	send_u8_decimal(dat->year - 2000);
	send_string("/");
	send_u8_decimal(dat->mon);
	send_string("/");
	send_u8_decimal(dat->day);
	send_string("/");
	
	send_u8_decimal(dat->hour);
	send_string("/");
	send_u8_decimal(dat->min);
	send_string("/");
	send_u8_decimal(dat->sec);
	send_string("/");
	
	send_u8_decimal(dat->week);
	send_string(" \r\n");
}

void prog(void)
{
    u8 i = 0;
	Date dat;
	u8 year, mon, day, hour, min, sec, week;
	
#ifdef IAP15W413AS_TEST
	ds3231_get_date(&year, &mon, &day, &hour, &min, &sec, &week);
	dat.year = year + 2000;
	dat.mon = mon;
	dat.day = day;
	dat.hour = hour;
	dat.min = min;
	dat.sec = sec;
	dat.week = week;
	print_time(&dat);
	bpc_enable();
	while (1) {
		if (get_bpc(&dat) == OK) {
			led_red_enable();
			print_time(&dat);
			year = dat.year - 2000;
			mon = dat.mon;
			day = dat.day;
			hour = dat.hour;
			min = dat.min;
			sec = dat.sec;
			week = dat.week;
			ds3231_set_date(year, mon, day, hour, min, sec, week);
		}
		delay_s(5);
		led_red_disable();
	}
	// bpc_disable();
#else
	while (1) {
		delay_s(1);
		if (i > 200) i = 1;
		send_u8_decimal(i);
		send_string(" \r\n");
		i++;
	}
#endif
}

int main(void)
{
	iap15w413as_config();

	send_string("IAP15W413AS DS3231SN Test. \r\n");
    
    prog();
    
    return 0;
}
