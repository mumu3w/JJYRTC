
#include "iap15w413as.h"

int leap_year(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) return TRUE;
    else return FALSE;
}

int day2date(Date *dat, int days)
{
    int days_per_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int day = 0, mon = 1;
    int i = 0;
#ifdef BPCDEBUG2
printf("year = %04d days = %d\n", dat->year, days);
#endif
    if (leap_year(dat->year)) days_per_month[1]++;
    while (1) {
        days -= days_per_month[i];
        if (days < 0) {
            days += days_per_month[i];
            day = days;
            mon = i + 1;
            break;
        }
        i++;
    }

    if (i > 11) return ERR;

    dat->day = day;
    dat->mon = mon;

    return OK;
}

void to_sec(Date *dat)
{
    static const int days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int year = dat->year;
    dat->timestamp = 0;

    dat->year -= 1900;
#ifdef BPCDEBUG2
printf("year = %04d \n", dat->year);
#endif
    dat->year -= (dat->year < 70 ? -30 : 70);
#ifdef BPCDEBUG2
printf("year = %04d \n", jjyd->year);
#endif
    dat->timestamp += (dat->year * YEAR);
    dat->timestamp += (((dat->year + 1) / 4) * DAY);
            
    dat->timestamp = dat->timestamp + days[dat->mon - 1] * DAY;
    if (leap_year(dat->year + 1970) && dat->mon > 2) dat->timestamp += DAY;
            
    dat->timestamp += ((dat->day - 1) * DAY);
    dat->timestamp += dat->hour * HOUR;
    dat->timestamp += dat->min * 60;
    dat->timestamp += dat->sec;

    dat->year = year;
}

void to_date(Date *dat)
{
    int days_per_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    long i, leap;
    long tstamp = dat->timestamp;
            
    for (i = 1970; ; i++) {
        if (leap_year(i)) {leap = DAY; } 
        else { leap = 0; }
        if ((dat->timestamp - YEAR - leap) < 0) { break; }
        dat->timestamp -= (YEAR + leap);
    }
    dat->year = i;
            
    if (leap_year(dat->year)) {days_per_month[1]++;}
    i = 0;
    while (dat->timestamp >= (days_per_month[i] * DAY)) {dat->timestamp -= (days_per_month[i++] * DAY);}
    dat->mon = i + 1;
            
    i = 1;
    while (dat->timestamp >= DAY) { dat->timestamp -= DAY; i++; }
    dat->day = i;
            
    i = 0;
    while (dat->timestamp >= HOUR) { dat->timestamp -= HOUR; i++; }
    dat->hour = i;
            
    i = 0;
    while (dat->timestamp >= 60L) { dat->timestamp -= 60L; i++; }
    dat->min = i;
            
    dat->sec = dat->timestamp;

    dat->timestamp = tstamp;
}

void to_week(Date *dat)
{
    int yyyy, mm, dd, week;

    yyyy = dat->year;
    mm = dat->mon;
    dd = dat->day;

    if (mm == 1 || mm == 2) {
        yyyy -= 1;
        mm += 12;
    }
    week = (dd + 2 * mm + 3 * (mm + 1) / 5 + yyyy + yyyy / 4 - yyyy / 100 + yyyy / 400) % 7;

    dat->week = week + 1;
}
