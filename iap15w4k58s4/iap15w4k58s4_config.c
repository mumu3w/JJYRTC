
#include "iap15w4k58s4.h"

void port_mode(void)
{
	P0M1 = 0x0;
	P0M0 = 0x0;
	
	P1M1 = 0x0;
	P1M0 = 0x0;
	
	P2M1 = 0x0;
	P2M0 = 0x0;
	
	P3M1 = 0x0;
	P3M0 = 0x0;
	
	P4M1 = 0x0;
	P4M0 = 0x0;
	
	P5M1 = 0x0;
	P5M0 = 0x0;
	
	P6M1 = 0x0;
	P6M0 = 0x0;
	
	P7M1 = 0x0;
	P7M0 = 0x0;
}

void iap15w4k58s4_config(void)
{
	port_mode();
	
	uart1_init();
	U1ENABLE();
	
	timer0_init();
	T0ENABLE();
	
	timer4_init();
	T4ENABLE();
	
	ISRENABLE();
	
	lcd1602_init();
	
	ds1302_init();
}
