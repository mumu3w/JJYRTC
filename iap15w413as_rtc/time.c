
#include "iap15w413as.h"

int leap_year(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) return TRUE;
    else return FALSE;
}

int day2date(Date *jjyd, int days)
{
    int days_per_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int day = 0, mon = 1;
    int i = 0;
#ifdef JJYDEBUG2
printf("year = %04d days = %d\n", jjyd->year, days);
#endif
    if (leap_year(jjyd->year)) days_per_month[1]++;
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

    jjyd->day = day;
    jjyd->mon = mon;

    return OK;
}

void to_sec(Date *jjyd)
{
    static const int days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int year = jjyd->year;
    jjyd->timestamp = 0;

    jjyd->year -= 1900;
#ifdef JJYDEBUG2
printf("year = %04d \n", jjyd->year);
#endif
    jjyd->year -= (jjyd->year < 70 ? -30 : 70);
#ifdef JJYDEBUG2
printf("year = %04d \n", jjyd->year);
#endif
    jjyd->timestamp += (jjyd->year * YEAR);
    jjyd->timestamp += (((jjyd->year + 1) / 4) * DAY);
            
    jjyd->timestamp = jjyd->timestamp + days[jjyd->mon - 1] * DAY;
    if (leap_year(jjyd->year + 1970) && jjyd->mon > 2) jjyd->timestamp += DAY;
            
    jjyd->timestamp += ((jjyd->day - 1) * DAY);
    jjyd->timestamp += jjyd->hour * HOUR;
    jjyd->timestamp += jjyd->min * 60;
    jjyd->timestamp += jjyd->sec;

    jjyd->year = year;
}

void to_date(Date *jjyd)
{
    int days_per_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    long i, leap;
    long tstamp = jjyd->timestamp;
            
    for (i = 1970; ; i++) {
        if (leap_year(i)) {leap = DAY; } 
        else { leap = 0; }
        if ((jjyd->timestamp - YEAR - leap) < 0) { break; }
        jjyd->timestamp -= (YEAR + leap);
    }
    jjyd->year = i;
            
    if (leap_year(jjyd->year)) {days_per_month[1]++;}
    i = 0;
    while (jjyd->timestamp >= (days_per_month[i] * DAY)) {jjyd->timestamp -= (days_per_month[i++] * DAY);}
    jjyd->mon = i + 1;
            
    i = 1;
    while (jjyd->timestamp >= DAY) { jjyd->timestamp -= DAY; i++; }
    jjyd->day = i;
            
    i = 0;
    while (jjyd->timestamp >= HOUR) { jjyd->timestamp -= HOUR; i++; }
    jjyd->hour = i;
            
    i = 0;
    while (jjyd->timestamp >= 60L) { jjyd->timestamp -= 60L; i++; }
    jjyd->min = i;
            
    jjyd->sec = jjyd->timestamp;

    jjyd->timestamp = tstamp;
}

void to_week(Date *jjyd)
{
    int yyyy, mm, dd, week;

    yyyy = jjyd->year;
    mm = jjyd->mon;
    dd = jjyd->day;

    if (mm == 1 || mm == 2) {
        yyyy -= 1;
        mm += 12;
    }
    week = (dd + 2 * mm + 3 * (mm + 1) / 5 + yyyy + yyyy / 4 - yyyy / 100 + yyyy / 400) % 7;

    jjyd->week = week;
}
