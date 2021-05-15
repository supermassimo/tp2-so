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
// TODO: Make it standard for all digits
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
void getTime(Time *dayTime, int utc){
	dayTime -> hours = getHourUTC(getRtc(Hours), utc);
    dayTime -> minutes = getRtc(Minutes);
    dayTime -> seconds = getRtc(Seconds);
}

// Prints date in format DD/MM/YYYY
void printDate(){
	Date date = {getRtc(MonthDay), getRtc(Month), getRtc(Year) + 2000};
	char day[3], month[3], year[5];
	numToStrSized(date.day, day, 10, 2);
	numToStrSized(date.month, month, 10, 2);
	numToStrSized(date.year, year, 10, 4);
	print(day);
	print("/");
	print(month);
	print("/");
	println(year);
}

// Fills the Date struct sent with current date
void getDate(Date *date){
	date -> day = getRtc(MonthDay);
	date -> month = getRtc(Month);
	date -> year = getRtc(Year) + 2000;
}