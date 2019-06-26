
#include "iap15w413as.h"

sbit LED_RED = P3^6;
sbit LDE_BLUE = P3^7;

void led_init(void)
{
	led_red_disable();
	led_blue_disable();
}

void led_red_enable(void)
{
	LED_RED = 0;
}

void led_red_disable(void)
{
	LED_RED = 1;
}

void led_blue_enable(void)
{
	LDE_BLUE = 0;
}

void led_blue_disable(void)
{
	LDE_BLUE = 1;
}
