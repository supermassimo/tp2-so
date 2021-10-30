// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <rtc.h>
#include <console.h>
#include <lib.h>

typedef enum RtcRegister{
	Seconds = 0,
	Minutes = 2,
	Hours = 4,
	WeekDay = 6,
	MonthDay = 7,
	Month = 8,
	Year = 9,
	RegA = 10,
	RegB = 11
} RtcRegister;

typedef enum Months{
	Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
} Months;

extern uint8_t getRtcReg(uint8_t reg);
extern uint8_t setRtcReg(uint8_t reg);

static uint8_t getHourUTC(uint8_t hour, int utc){
	int newHour = hour + utc;
	if(newHour >= 24)
		return newHour - 24;
	if(newHour < 0)
		return newHour + 24;
	return newHour;
}

// 2 digit version
static int bcdToBinary(int value){
	return ((value / 16) * 10) + (value & 0x0F);
}

uint8_t getRtc(uint8_t reg){
	return bcdToBinary(getRtcReg(reg));
}

// Prints daytime in format HH:MM:SS (24hs)
void printTime(int utc){
	Time dayTime = {getHourUTC(getRtc(Hours), utc), getRtc(Minutes), getRtc(Seconds)};
	char hours[3], minutes[3], seconds[3];
	numToStrSized(dayTime.hours, hours, 10, 2);
	numToStrSized(dayTime.minutes, minutes, 10, 2);
	numToStrSized(dayTime.seconds, seconds, 10, 2);
	print(hours);
	print(":");
	print(minutes);
	print(":");
	println(seconds);
}

// Fills the Time struct sent with current daytime
// Returns whether the day was changed or not
//  0 -> day=utcDay
//  1 -> day=utcDay+1
// -1 -> day=utcDay-1
int getTime(Time *dayTime, int utc){
	int dayChange = 0;
	int newHour = getRtc(Hours) + utc;
	if(newHour >= 24){
		newHour -= 24;
		dayChange = 1;
	}
	if(newHour < 0){
		newHour += 24;
		dayChange = -1;
	}
	dayTime -> hours = newHour;
    dayTime -> minutes = getRtc(Minutes);
    dayTime -> seconds = getRtc(Seconds);
	return dayChange;
}

static int has30Days(int month){
	return month == Apr || month == Jun || month == Sep || month == Nov;
}

static int isLeapYear(int year){
	return year % 4 == 0 && (year/100) % 4 == 0;
}

// Fills the Date struct sent with current date
void getDate(Date *date){
	date -> day = getRtc(MonthDay);
	date -> month = getRtc(Month);
	date -> year = getRtc(Year) + 2000;
}

// Gets current date adding daySkip or not as indicated
void getDateChanged(Date *date, int daySkip){
	getDate(date);
	switch(daySkip){
		case 1:
			if(date->day == 28 && date->month == Feb){
				if(isLeapYear(date->year))
					date->day = 29;
				else{
					date->day = 1;
					date->month = Mar;
				}
			}
			else if(date->day == 30 && has30Days(date->month)){
				date->day = 1;
				date->month += 1;
			}
			else if(date->day == 31){
				if(date->month == Dec){
					date->day = 1;
					date->month = Jan;
					date->year += 1;
				}
				else{
					date->day = 1;
					date->month += 1;
				}
			}
			else{
				date->day += 1;
			}
			break;
		case -1:
			if(date->day == 1){
				if(date->month == Jan){
					date->day = 31;
					date->month = Dec;
					date->year -= 1;
				}
				else {
					if(date->month == Mar){
						if(!isLeapYear(date->year))
							date-> day = 28;
						else
							date->day = 29;
					}
					else {
						if(has30Days(date->month-1))
							date->day = 30;
						else
							date->day = 31;
					}
					date->month -= 1;
				}
			}
			else{
				date->day -= 1;
			}
			break;
		default:
			break;
	}
}