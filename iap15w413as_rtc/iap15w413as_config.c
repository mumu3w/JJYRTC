
#include "iap15w413as.h"

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

void iap15w413as_config(void)
{
	port_mode();
	
	uart1_init();
	U1ENABLE();
	
	timer0_init();
	T0ENABLE();
	
	ISRENABLE();
	
	ds3231_init();
}
