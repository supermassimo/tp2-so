#ifndef RTC_H
#define RTC_H

#include <stdint.h>

typedef struct Time{
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} Time;

typedef struct Date{
	uint8_t day;
	uint8_t month;
	uint16_t year;
} Date;

void getTime(Time *dayTime, int utc);
void printTime(int utc);
void getDate(Date *date);
void printDate();

#endif