
#include "iap15w413as.h"

sbit bpc_go = P3^5;
sbit bpc_pin = P3^4;

void bpc_config(void)
{
	bpc_pin = LOWER;
	PINMxSET(P3M1, 4);
	PINMxCLS(P3M0, 4);
	
	bpc_disable();
}

void bpc_enable(void)
{
	bpc_go = 0;
}

void bpc_disable(void)
{
	bpc_go = 1;
}

static int bpc_sync(void)
{
	int err = 0;
	
	while (bpc_pin != HIGH) {
		delay_ms(10);
        if (err >= 200) return ERR;   // 2秒内没收到高平脉冲,返回错误.
        err++;
	}
	
	err = 0;
    while (bpc_pin != LOWER) {
        delay_ms(10);
        if (err >= 100) return ERR;   // 2秒内没收到低平脉冲,返回错误.
        err++;
    }

    return OK;
}

static int bpc_sec(void)
{
    u16 H = 0;
    u16 L = 0;
    u16 T;
    u8 i = 0;

    while (i < 100) {
        if (bpc_flag == 1) {
            T = bpc_pin;
            if (T) {
				H++; 
				led_color_enable();
			} else {
				L++; 
				led_color_disable();
			}

            i++;
            bpc_flag = 0;
        }
    }
#ifdef IAP15W413AS_TEST
	send_u8_decimal(L);
	send_string("0  \t\t");
#endif
    if (L >= 85 && L < 95) return '0';		// 100ms
    if (L >= 75 && L < 85) return '1';		// 200ms
    if (L >= 65 && L < 75) return '2';		// 300ms
	if (L >= 55 && L < 65) return '3';		// 400ms
	if (L >= 95) return 'P';                // 1000ms

    return ERR;
}

static int to_coding(int ch)
{
#ifdef IAP15W413AS_TEST
	send_data(ch);
    send_string("  \r\n");
#endif
    if (ch == '0') return 0;
    if (ch == '1') return 1;
	if (ch == '2') return 2;
	if (ch == '3') return 3;
    if (ch == 'P') return 4;

    return ERR;
}

static int bpc_check(u8 *bpcarray)
{
    u8 val;
    u8 bits = 0;
    int i, j;

    for (i = 1; i < 10; i++) {
        val = bpcarray[i];
        for (j = 0; j < 8; j++) {
            if ((val >> j) & 0x1 == 1) bits++;
        }
    }

    bits %= 2;
    if (bits != bpcarray[10] && bits+2 != bpcarray[10]) return ERR;

    bits = 0;
    for (i = 11; i < 19; i++) {
        val = bpcarray[i];
        for (j = 0; j < 8; j++) {
            if ((val >> j) & 0x1 == 1) bits++;
        }
    }

    bits %= 2;
    if (bits != bpcarray[19] && bits+2 != bpcarray[19]) return ERR;

    return OK;
}

static void bpc2time(u8 *bpcarray, Date *dat)
{
    dat->sec = bpcarray[1] * 20;
    dat->min = (bpcarray[5] * 4 + bpcarray[6]) * 4 + bpcarray[7];
    dat->hour = bpcarray[3] * 4 + bpcarray[4];

    dat->week = bpcarray[8] * 4 + bpcarray[9];
    if (bpcarray[10] == 2 || bpcarray[10] == 3)
        if (dat->hour != 12)
            dat->hour += 12;

    dat->day = (bpcarray[11] * 4 + bpcarray[12]) * 4 + bpcarray[13];
    dat->mon = bpcarray[14] * 4 + bpcarray[15];
    dat->year = (bpcarray[16] * 4 + bpcarray[17]) * 4 + bpcarray[18];
    if (bpcarray[19] == 2 || bpcarray[19] == 3)
        dat->year += 0x40;
	dat->year += 2000;
}

#define BPCSIZE		20
int get_bpc(Date *dat)
{
	u8 bpc_array[2][BPCSIZE] = {0};
	Date dat_array[2];
	int status;
	int coding;
	int i = 0;
	int k = 0;
	int j = 0;
	
	while (1) {
		status = bpc_sync();
#ifdef IAP15W413AS_TEST
        if (status == OK) send_string("Ok. \r\n");
		else send_string("Err. \r\n");
#endif
		if (status != ERR) break;
		if (i >= 5) return ERR;
		i++;
	}
	
	while (1) {
		if (k > BPCSIZE) return ERR;
		coding = to_coding(bpc_sec());
		if (coding == ERR) return ERR;
		if (coding == 4) break;
		k++;
	}
	
	for (j = 0; j < 2; j++) {
		i = 0;
		bpc_array[j][i] = 4;	// 'P'
		i++;
		while (1) {
			coding = to_coding(bpc_sec());
			if (coding == ERR) return ERR;
			if (coding == 4) {
				if (i == BPCSIZE) {
					if (bpc_check(&bpc_array[j][0]) == OK) break;
					else {send_string("check err. \r\n");return ERR;}
				}
				else return ERR;
			}
			
			if (i >= BPCSIZE) return ERR;
			bpc_array[j][i] = coding;
			i++;
		}
		send_string("T. \r\n");
	}
	
	bpc2time(&bpc_array[0][0], &dat_array[0]);
    bpc2time(&bpc_array[1][0], &dat_array[1]);
	to_sec(&dat_array[0]); to_sec(&dat_array[1]);
	if ((dat_array[1].timestamp - dat_array[0].timestamp) == 20) {
		*dat = dat_array[1];
		dat->timestamp += 21;
		to_date(dat);
		to_week(dat);
		return OK;
	} else return ERR;
}
