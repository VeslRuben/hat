#ifndef HAT_TIMELIB_H
#define HAT_TIMELIB_H

#include <cinttypes>

#ifndef __AVR__
#include <sys/types.h> // for __time_t_defined, but avr libc lacks sys/types.h
#endif

typedef struct {
    uint8_t Second;
    uint8_t Minute;
    uint8_t Hour;
    uint8_t Wday;   // day of week, sunday is day 1
    uint8_t Day;
    uint8_t Month;
    uint8_t Year;   // offset from 1970;
} tmElements_t, TimeElements, *tmElementsPtr_t;

//convenience macros to convert to and from tm years
#define  tmYearToCalendar(Y) ((Y) + 1970)  // full four digit year

// leap year calculator expects year argument as years offset from 1970
#define LEAP_YEAR(Y)     ( ((1970+(Y))>0) && !((1970+(Y))%4) && ( ((1970+(Y))%100) || !((1970+(Y))%400) ) )

typedef time_t(*getExternalTime)();


/*==============================================================================*/
/* Useful Constants */
#define SECS_PER_MIN  ((time_t)(60UL))
#define SECS_PER_HOUR ((time_t)(3600UL))
#define SECS_PER_DAY  ((time_t)(SECS_PER_HOUR * 24UL))
#define DAYS_PER_WEEK ((time_t)(7UL))
#define SECS_PER_WEEK ((time_t)(SECS_PER_DAY * DAYS_PER_WEEK))

/* Useful Macros for getting elapsed time */
#define dayOfWeek(_time_) ((((_time_) / SECS_PER_DAY + 4)  % DAYS_PER_WEEK)+1) // 1 = Sunday
#define elapsedSecsToday(_time_) ((_time_) % SECS_PER_DAY)   // the number of seconds since last midnight
// The following macros are used in calculating alarms and assume the clock is set to a date later than Jan 1 1971
// Always set the correct time before setting alarms
#define previousMidnight(_time_) (((_time_) / SECS_PER_DAY) * SECS_PER_DAY)  // time at the start of the given day
#define elapsedSecsThisWeek(_time_) (elapsedSecsToday(_time_) +  ((dayOfWeek(_time_)-1) * SECS_PER_DAY))   // note that week starts on day 1
#define previousSunday(_time_) ((_time_) - elapsedSecsThisWeek(_time_))      // time at the start of the week for the given time



/*============================================================================*/

static uint32_t sysTime = 0;
static uint32_t prevMillis = 0;
static uint32_t millisOffset = 0;
static uint32_t sysTimeMillis = 0;

/*  time and date functions   */
int hour();            // the hour now
int hour(time_t t);    // the hour for the given time
int minute();          // the minute now
int minute(time_t t);  // the minute for the given time
int second();          // the second now
int second(time_t t);  // the second for the given time
int milliseconds();
int milliseconds(time_t t);
int day();             // the day now
int day(time_t t);     // the day for the given time
int weekday();         // the weekday now (Sunday is day 1)
int weekday(time_t t); // the weekday for the given time
int month();           // the month now  (Jan is month 1)
int month(time_t t);   // the month for the given time
int year();            // the full four digit year: (2009, 2010 etc)
int year(time_t t);    // the year for the given time

time_t now();              // return the current time as seconds since Jan 1 1970
void setTime(time_t t);
void setTime(int hr, int min, int sec, int milSec, int day, int month, int yr);
void adjustTime(long adjustment);

/* date strings */
#define dt_MAX_STRING_LEN 9 // length of longest date string (excluding terminating null)
char *monthStr(uint8_t month);
char *dayStr(uint8_t day);
char *monthShortStr(uint8_t month);
char *dayShortStr(uint8_t day);


/* low level functions to convert to and from system time                     */
void breakTime(time_t time, tmElements_t &tm);  // break time_t into elements
time_t makeTime(const tmElements_t &tm);  // convert time elements into time_t

#endif //HAT_TIMELIB_H
