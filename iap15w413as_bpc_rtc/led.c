
#include "iap15w413as.h"

sbit led_red = P3^7;
sbit led_color = P3^6;

void led_config(void)
{
	led_red = 1;
	led_color = 1;
}

void led_red_enable(void)
{
	led_red = 0;
}

void led_red_disable(void)
{
	led_red = 1;
}

void led_color_enable(void)
{
	led_color = 0;
}

void led_color_disable(void)
{
	led_color = 1;
}
