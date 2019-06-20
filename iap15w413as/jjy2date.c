
#include "iap15w413as.h"

sbit JJYPIN = P1^2;
sbit JJYKHz = P1^1;
sbit JJYGo = P1^0;

#define JJY60KHZ			1
#define JJY40KHZ			0
#define JJYENABLE 			0
#define JJYDISABLE			1

void jjy_khz(u8 khz)
{
	if (khz == 60) JJYKHz = JJY60KHZ;
	if (khz == 40) JJYKHz = JJY40KHZ;
}

void jjy_enable(void)
{
	JJYGo = JJYENABLE;
}

void jjy_disable(void)
{
	JJYGo = JJYDISABLE;
}

static int sync_sec(void)
{
	int err = 0;

    while (JJYPIN != HIGH) {
        delay_ms(10);
        if (err >= 100) return ERR;   // 1秒内没收到高平脉冲,返回错误.
        err++;
    }
    err = 0;
    while (JJYPIN != LOWER) {
        delay_ms(10);
        if (err >= 100) return ERR;   // 1秒内没收到低平脉冲,返回错误.
        err++;
    }

    return OK;
}

static int jjy_sec(void)
{
    u16 H = 0;
    u16 L = 0;
    u16 T;
    u8 i = 0;

    while (i < 100) {
        if (JJYstatus == 1) {
            T = JJYPIN;
            if (T) {
				H++; // led_blue_enable();
			} else {
				L++; // led_blue_disable();
			}

            i++;
            JJYstatus = 0;
        }
    }
#ifdef JJYDEBUG
	send_u8_decimal(L);
	send_string("0  \t\t");
#endif
    if (L >= 15 && L <= 25) return 'P';		// 200ms
    if (L >= 75 && L <= 85) return '0';		// 800ms
    if (L >= 45 && L <= 55) return '1';		// 500ms

    return ERR;
}

static int to_coding(int ch)
{
#ifdef JJYDEBUG
	send_data(ch);
    send_string("  \r\n");
#endif
    if (ch == '0') return 0;
    if (ch == '1') return 1;
    if (ch == 'P') return 2;

    return ERR;
}

void jjy_config(void)
{
	JJYPIN = LOWER;
	PINMxSET(P1M1, 2);
	PINMxCLS(P1M0, 2);
	
	jjy_disable();
}

static int checkjjy(u8 *jjycoding)
{
    int pa1 = jjycoding[36];
    int pa2 = jjycoding[37];
    int i = 0;

    if (jjycoding[1] == 1) i++;
    if (jjycoding[2] == 1) i++;
    if (jjycoding[3] == 1) i++;
    if (jjycoding[5] == 1) i++;
    if (jjycoding[6] == 1) i++;
    if (jjycoding[7] == 1) i++;
    if (jjycoding[8] == 1) i++;
    if (i % 2 != pa2) return ERR;

    i = 0;
    if (jjycoding[12] == 1) i++;
    if (jjycoding[13] == 1) i++;
    if (jjycoding[15] == 1) i++;
    if (jjycoding[16] == 1) i++;
    if (jjycoding[17] == 1) i++;
    if (jjycoding[18] == 1) i++;
    if (i % 2 != pa1) return ERR;

    if (!(jjycoding[0] == 2 && jjycoding[9] == 2 && jjycoding[19] == 2 && 
        jjycoding[29] == 2 && jjycoding[39] == 2 && jjycoding[49] == 2 && 
        jjycoding[59] == 2)) return ERR;

    if (!(jjycoding[4] == 0 && jjycoding[10] == 0 && jjycoding[11] == 0 && 
        jjycoding[14] == 0 && jjycoding[20] == 0 && jjycoding[21] == 0 && 
        jjycoding[24] == 0 && jjycoding[34] == 0 && jjycoding[35] == 0 && 
        jjycoding[55] == 0 && jjycoding[56] == 0 && jjycoding[57] == 0 && 
        jjycoding[58] == 0)) return ERR;

    return OK;
}

static int bcd2hex_(int bcd)
{
    return ((bcd >> 8) & 0xf) * 100 + ((bcd >> 4) & 0xf) * 10 + (bcd & 0xf);
}

int jjy2date(Date *jjyd, u8 *jjycoding)
{
    int min, hour, days, year, week;

    if (checkjjy(jjycoding)) return ERR;

    min = ((jjycoding[1] * 2 + jjycoding[2]) * 2 + jjycoding[3]) * 16;
    min += ((jjycoding[5] * 2 + jjycoding[6]) * 2 + jjycoding[7]) * 2 + jjycoding[8];
    jjyd->min = bcd2hex_(min);

    hour = (jjycoding[12] * 2 + jjycoding[13]) * 16;
    hour += ((jjycoding[15] * 2 + jjycoding[16]) * 2 + jjycoding[17]) * 2 + jjycoding[18];
    jjyd->hour = bcd2hex_(hour);

    year = (((jjycoding[41] * 2 + jjycoding[42]) * 2 + jjycoding[43]) * 2 + jjycoding[44]) * 16;
    year += ((jjycoding[45] * 2 + jjycoding[46]) * 2 + jjycoding[47]) * 2 + jjycoding[48];
    jjyd->year = bcd2hex_(year) + 2000;

    days = (jjycoding[22] * 2 + jjycoding[23]) * 256;
    days += (((jjycoding[25] * 2 + jjycoding[26]) * 2 + jjycoding[27]) * 2 + jjycoding[28]) * 16;
    days += ((jjycoding[30] * 2 + jjycoding[31]) * 2 + jjycoding[32]) * 2 + jjycoding[33];
    day2date(jjyd, bcd2hex_(days));

    week = (jjycoding[51] * 2 + jjycoding[52]) * 2 + jjycoding[52];
    jjyd->week = bcd2hex_(week);

    jjyd->sec = 0;
    // jjyd->timestamp = 0;
    // jjyd->hour -= 1;             // UTC+8 (JJY UTC+9)
    to_sec(jjyd);
    jjyd->timestamp += 60;          // 一帧结束时已经过去60秒
    jjyd->timestamp += 1;           // 在GetJJY中,以遇到两个定位符为一帧的结束,此时已经是后续帧的第一秒了.
    jjyd->timestamp -= (60 * 60);   // UTC+8 (JJY UTC+9)
    to_date(jjyd);
    to_week(jjyd);

    return OK;
}
#ifdef JJYDEBUG2
void printdate(Date *jjyd)
{
    const char *weekname[] = {"Monday", "Tuesday", "Wednesday", "Thursday",
                                    "Friday", "Saturday", "Sunday", NULL};

    printf("%04d/%02d/%02d \r\n", jjyd->year, jjyd->mon, jjyd->day);
    printf("%02d:%02d:%02d \r\n", jjyd->hour, jjyd->min, jjyd->sec);
    printf("%s\r\n", weekname[jjyd->week]);
}
#endif

int GetJJY(Date *jjyd)
{
    int status;
	int i = 0;
    int j = 0;
    int k = 0;
    int coding = 0, prev_coding = 0;
	u8 jjycoding[JJYSIZE];
	
    while (1) {
		status = sync_sec();
#ifdef JJYDEBUG
        if (status == OK) send_string("Ok. \r\n");
		else send_string("Err. \r\n");
#else
#endif
		if (status != ERR) break;
		if (i >= 5) return ERR;
		i++;
	}

    while (1) {
        if (k >= JJYSIZE) return ERR;
        coding = to_coding(jjy_sec());
        if (coding == ERR) return ERR;
        if (coding == 2)
            if (prev_coding == 2)
                break;
        k++;
        prev_coding = coding;
    }
#ifdef JJYDEBUG
    send_string("Go. \r\n");
#endif
    jjycoding[0] = 2;
    j = 1;
    while (1) {
        coding = to_coding(jjy_sec());
        if (coding == ERR) return ERR;
        if (coding == 2)
            if (jjycoding[j-1] == 2)
                    break;
        if (!(j < JJYSIZE)) return ERR;
        jjycoding[j] = coding;
        j++;
    }

    if (jjy2date(jjyd, jjycoding)) return ERR;
    return OK;
}

