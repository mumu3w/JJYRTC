
#include "iap15w413as.h"

void print_time(Date *jjyd)
{
	// const char *_week_[] = {"Mon ", "Tues", "Wed ", 
	//						"Thur", "Fri ", "Sat ", "Sun "};
	
	send_u8_decimal(jjyd->year - 2000);
	send_string("/");
	send_u8_decimal(jjyd->mon);
	send_string("/");
	send_u8_decimal(jjyd->day);
	send_string("/");
	
	send_u8_decimal(jjyd->hour);
	send_string("/");
	send_u8_decimal(jjyd->min);
	send_string("/");
	send_u8_decimal(jjyd->sec);
	send_string("/");
	
	//send_string(_week_[jjyd->week]);
	send_u8_decimal(jjyd->week);
	send_string(" \r\n");
}

void main(void)
{
	u8 i = 0;
	Date jjyd;
	
	iap15w413as_config();
#ifdef JJYDEBUG	
	send_string("IAP15W413AS JJY Test.                   \r\n");
#endif
	jjy_khz(60);
	jjy_enable();
	while (1) {
		if (GetJJY(&jjyd)) {
#ifdef JJYDEBUG
			send_string("Failure. ");
#else
			send_string("F. ");
#endif
			send_string("000-000-000-000-000-000-000 \r\n");	// JJY_TIME_MESS_SIZE = 33
			led_blue_enable();
		} else {
#ifdef JJYDEBUG
			send_string("Success. "); 
#else
			send_string("S. "); 
#endif
			print_time(&jjyd);
			led_red_enable();
		}
	}
	// jjy_disable();
}
