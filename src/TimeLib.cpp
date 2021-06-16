#include "TimeLib.h"
#include "wiring_time.h"

namespace TimeLib {
    static tmElements_t tm;          // a cache of time elements
    static time_t cacheTime;   // the time the cache was updated
    static uint32_t syncInterval = 300;  // time sync will be attempted after this many seconds

    void refreshCache(time_t t) {
        if (t != cacheTime) {
            breakTime(t, tm);
            cacheTime = t;
        }
    }

    int milliseconds() {
        return ((int) millis() + (int) millisOffset) % 1000;
    }

    int milliseconds(time_t t) {
        return ((int) sysTimeMillis + (int) millisOffset) % 1000;
    }

    int hour() { // the hour now
        return hour(now());
    }

    int hour(time_t t) { // the hour for the given time
        refreshCache(t);
        return tm.Hour;
    }

    int minute() {
        return minute(now());
    }

    int minute(time_t t) { // the minute for the given time
        refreshCache(t);
        return tm.Minute;
    }

    int second() {
        return second(now());
    }

    int second(time_t t) {  // the second for the given time
        refreshCache(t);
        return tm.Second;
    }

    int day() {
        return (day(now()));
    }

    int day(time_t t) { // the day for the given time (0-6)
        refreshCache(t);
        return tm.Day;
    }

    int weekday() {   // Sunday is day 1
        return weekday(now());
    }

    int weekday(time_t t) {
        refreshCache(t);
        return tm.Wday;
    }

    int month() {
        return month(now());
    }

    int month(time_t t) {  // the month for the given time
        refreshCache(t);
        return tm.Month;
    }

    int year() {  // as in Processing, the full four digit year: (2009, 2010 etc)
        return year(now());
    }

    int year(time_t t) { // the year for the given time
        refreshCache(t);
        return tmYearToCalendar(tm.Year);
    }

/*============================================================================*/
/* functions to convert to and from system time */
/* These are for interfacing with time services and are not normally needed in a sketch */

    static const uint8_t monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,
                                        31}; // API starts months from 1, this array starts from 0

    void breakTime(time_t timeInput, tmElements_t &tm) {
// break the given time_t into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

        uint8_t year;
        uint8_t month, monthLength;
        uint32_t time;
        unsigned long days;

        time = (uint32_t) timeInput;
        tm.Second = time % 60;
        time /= 60; // now it is minutes
        tm.Minute = time % 60;
        time /= 60; // now it is hours
        tm.Hour = time % 24;
        time /= 24; // now it is days
        tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1

        year = 0;
        days = 0;
        while ((unsigned) (days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
            year++;
        }
        tm.Year = year; // year is offset from 1970

        days -= LEAP_YEAR(year) ? 366 : 365;
        time -= days; // now it is days in this year, starting at 0

        days = 0;
        month = 0;
        monthLength = 0;
        for (month = 0; month < 12; month++) {
            if (month == 1) { // february
                if (LEAP_YEAR(year)) {
                    monthLength = 29;
                } else {
                    monthLength = 28;
                }
            } else {
                monthLength = monthDays[month];
            }

            if (time >= monthLength) {
                time -= monthLength;
            } else {
                break;
            }
        }
        tm.Month = month + 1;  // jan is month 1
        tm.Day = time + 1;     // day of month
    }

    time_t makeTime(const tmElements_t &tm) {
// assemble time elements into time_t
// note year argument is offset from 1970 (see macros in time.h to convert to other formats)
// previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9

        int i;
        uint32_t seconds;

        // seconds from 1970 till 1 jan 00:00:00 of the given year
        seconds = tm.Year * (SECS_PER_DAY * 365);
        for (i = 0; i < tm.Year; i++) {
            if (LEAP_YEAR(i)) {
                seconds += SECS_PER_DAY;   // add extra days for leap years
            }
        }

        // add days for this year, months start from 1
        for (i = 1; i < tm.Month; i++) {
            if ((i == 2) && LEAP_YEAR(tm.Year)) {
                seconds += SECS_PER_DAY * 29;
            } else {
                seconds += SECS_PER_DAY * monthDays[i - 1];  //monthDay array starts from 0
            }
        }
        seconds += (tm.Day - 1) * SECS_PER_DAY;
        seconds += tm.Hour * SECS_PER_HOUR;
        seconds += tm.Minute * SECS_PER_MIN;
        seconds += tm.Second;
        return (time_t) seconds;
    }
/*=====================================================*/
/* Low level system time functions  */

    time_t now() {
        // calculate number of seconds passed since last call to now()
        while (millis() - prevMillis >= 1000) {
            // millis() and prevMillis are both unsigned ints thus the subtraction will always be the absolute value of the difference
            sysTime++;
            prevMillis += 1000;
        }
        sysTimeMillis = millis();
        return (time_t) sysTime;
    }

    void setTime(time_t t) {
        sysTime = (uint32_t) t;
        prevMillis = millis();  // restart counting from now (thanks to Korman for this fix)
    }

    void setTime(int hr, int min, int sec, int milSec, int dy, int mnth, int yr) {
        // year can be given as full four digit year or two digts (2010 or 10 for 2010);
        //it is converted to years since 1970
        if (yr > 99)
            yr = yr - 1970;
        else
            yr += 30;
        tm.Year = yr;
        tm.Month = mnth;
        tm.Day = dy;
        tm.Hour = hr;
        tm.Minute = min;
        tm.Second = sec;
        setTime(makeTime(tm));
        millisOffset = milSec;
    }

    void adjustTime(long adjustment) {
        sysTime += adjustment;
    }
}