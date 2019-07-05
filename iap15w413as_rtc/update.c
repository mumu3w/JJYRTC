
#include "iap15w413as.h"

#define TIME_START_BIT	3	
int ascii2hex(u8 *mess, u8 *year, u8 *mon, u8 *day, 
				u8 *hour, u8 *min, u8 *sec, u8 *week)
{
	u8 i = 0;
	u8 v[7];
	
	if (mess[0] == 'S' &&
		mess[1] == '.' &&
		mess[31] == '\r' &&
		mess[32] == '\n') {
		
		send_string(mess);
		for (i = 0; i < 7; i++) {
			v[i] = (mess[TIME_START_BIT + i * 4 + 1] - '0') * 10;
			//send_u8_decimal(v[i]);
			//send_string(" \r\n");
			v[i] = v[i] + (mess[TIME_START_BIT + i * 4 + 2] - '0');
			//send_u8_decimal(v[i]);
			//send_string(" \r\n");
		}
		*year = v[0];
		*mon = v[1];
		*day = v[2];
		*hour = v[3];
		*min = v[4];
		*sec = v[5];
		*week = v[6];
		return OK;	
	} else {
		return ERR;
	}
}

int update_time(void)
{
	int return_code = ERR;
	u8 year, mon, day;
	u8 hour, min, sec;
	u8 week;
	
	if (JjyTimeMessFlag == 1) {
		JjyTimeMessFlag = 0;
		return_code = ascii2hex(jjy_time_mess, &year, &mon, &day,
				&hour, &min, &sec, &week);
	}
	
	if (return_code == OK) {
		ds3231_set_date(year, mon, day, hour, min, sec, week + 1);
		ds1302_set_date(year, mon, day, hour, min, sec, week + 1);
		return OK;
	}
	
	return ERR;
}
