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