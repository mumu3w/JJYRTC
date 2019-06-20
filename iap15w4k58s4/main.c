
#include "iap15w4k58s4.h"
#include <stdio.h>

void prog(void)
{
	u8 old_sec = 0;
	u8 time_str_time[17];
	//u16 year, mon, day, hour, min, sec, week;
	u8 update_flag = 1;

	while (1) {
		delay_ms(200);
		if (old_sec != sec1 && day1 != 0) {
			//printf("20%02u-%u-%u \r\n", year1, mon1, day1);
			//printf("%u:%u:%u \r\n", hour1, min1, sec1);
			//printf("%u \r\n", week1);
			
			//ds1302_get_time(&year, &mon, &day, &hour, &min, &sec, &week);
			sprintf(time_str_time, "   20%02u-%02u-%02u   ", year1, mon1, day1);
			send_string(time_str_time);
			lcd1602_pos(0, 0);
			lcd1602_disp(time_str_time, 16);
			sprintf(time_str_time, "    %02u:%02u:%02u    ", hour1, min1, sec1);
			send_string(time_str_time);
			lcd1602_pos(0, 1);
			lcd1602_disp(time_str_time, 16);
			
			old_sec = sec1;
		}
		
		if (update_flag) {
			if (update_time() == OK)
				update_flag = 0;
		}
	}
}

int main(void)
{
	iap15w4k58s4_config();
	
	send_string("IAP15W4K58S4 JJY Test. \r\n");
	
	prog();
	
	return 0;
}
