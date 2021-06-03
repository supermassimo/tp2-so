#include <console.h>
#include <lib.h>
#include <rtc.h>

#define SCR_BASE_ADDR 	0xB8000
#define SCR_ROWS 		25
#define SCR_COLS 		80

#define MAX_NUM_LENGTH 255					// For printing numbers purposes

static char* scrPos = SCR_BASE_ADDR; 	// Current position on the screen
static uint8_t foreColor = White;			// Default Forecolor
static uint8_t backColor = Black;			// Default Backcolor
static uint8_t errorColor = Red;

static void fillScreen(uint8_t colorByte){
	char* current = SCR_BASE_ADDR + 1;
	int finalPos = SCR_ROWS * SCR_COLS;
	for(int i=0 ; i < finalPos ; i++){
		*current = colorByte;
		current += 2;
	}
}

void newLine(){
	int current = scrPos - SCR_BASE_ADDR;
	current /= SCR_COLS * 2;
	scrPos = SCR_BASE_ADDR + (current+1) * SCR_COLS * 2;
}

static uint8_t getColorByte(){
	return backColor << 4 | foreColor;
}

static uint8_t getColorByteCustom(uint8_t foreColor, uint8_t backColor){
	return backColor << 4 | foreColor;		// NO TOCAR
}

void setForeColor(uint8_t color){
	foreColor = color;
	fillScreen(getColorByte());
}

void setBackColor(uint8_t color){
	backColor = color;
	fillScreen(getColorByte());
}

void setErrorColor(uint8_t color){
	errorColor = color;
}

// Returns the previous char address omitting 'empty' chars
static uint64_t getPrevCharAddr(){
	char* prevChar = scrPos;
	while(*prevChar == 0){
		prevChar -= 2;
	}
	return (uint64_t)prevChar;
}

static int isSpecialChar(char c){
	int res;
	switch(c){
		case '\n': case '\b':
			res = 1;
			break;
		default:
			res = 0;
			break;
	}
	return res;
}

static void printSpecialChar(char c, uint8_t colorByte){
	char* finalPos;
	switch(c){
		case '\n':
			newLine();
			break;
		case '\b':
			if(scrPos-2 >= SCR_BASE_ADDR){			// Check if there are chars to delete
				finalPos = getPrevCharAddr();
				*finalPos = *(finalPos+2);			// Backspace functionality
				*(finalPos+1) = getColorByte();		// Fill current char with next one and move cursor one place back
				scrPos = finalPos;
			}
			break;
		default:
			break;
	}
}

static void scrollUp(){
	char *current = SCR_BASE_ADDR;
	// Muevo todos los caracteres (de la linea 1 a la 25) una posicion hacia arriba
	for(int i=1 ; i < SCR_ROWS ; i++){
		for(int j=0 ; j < SCR_COLS ; j++){
			*current = *(current+SCR_COLS*2);
			*(current+1) = *(current+SCR_COLS*2+1);
			current += 2;
		}
	}
	// Limpio la última línea, dejando el color previamente usado
	for(int j=0 ; j < SCR_COLS ; j++){
		*current = 0;
		current += 2;
	}
	// Resetting the cursor to first char of last row
	scrPos = SCR_BASE_ADDR + (SCR_ROWS-1)*SCR_COLS*2;
}

void printChar(char c){
	printCharCol(c, foreColor, backColor);
}

void printCharCol(char c, uint8_t foreColor, uint8_t backColor){
    char colorByte = getColorByteCustom(foreColor, backColor);
	if(scrPos >= SCR_BASE_ADDR + SCR_ROWS * SCR_COLS * 2){
        scrollUp();
    }
	if(isSpecialChar(c)){
		printSpecialChar(c, colorByte);
	}
    else{
        *scrPos = c;
        *(scrPos+1) = colorByte;
        scrPos += 2;
    }
}

void printCol(char* msg, uint8_t foreColor, uint8_t backColor){
	for(int i=0 ; msg[i] != 0 ; i++){
		printCharCol(msg[i], foreColor, backColor);
	}
}

void print(char* msg){
	printCol(msg, foreColor, backColor);
}

void printlnCol(char* msg, uint8_t foreColor, uint8_t backColor){
	printCol(msg, foreColor, backColor);
	newLine();
}

void println(char* msg){
	printlnCol(msg, foreColor, backColor);
}

void printInt(long num, size_t base){
	printIntCol(num, base, foreColor, backColor);
}

void printIntCol(long num, size_t base, uint8_t foreColor, uint8_t backColor){
	char strNum[MAX_NUM_LENGTH];
	numToStr(num, strNum, base);
	printCol(strNum, foreColor, backColor);
}

void printErr(char* msg){
	printlnCol(msg, errorColor, backColor);
}

void clearScreen(){
	char* scr = SCR_BASE_ADDR;
	for(int i=0 ; i < SCR_ROWS ; i++){
		for(int j=0 ; j < SCR_COLS ; j++){
			*(scr) = 0;
			*(scr+1) = 0;
			scr += 2;
		}
	}
	scrPos = SCR_BASE_ADDR;
}

void printRegistries(){
	uint64_t array[15];
	getRegistries(array, 15);
	print("RAX: ");
	printInt(array[0], 10);
	newLine();
	print("RBX: ");
	printInt(array[1], 10);
	newLine();
	print("RCX: ");
	printInt(array[2], 10);
	newLine();
	print("RDX: ");
	printInt(array[3], 10);
	newLine();
	print("RBP: ");
	printInt(array[4], 10);
	newLine();
	print("RDI: ");
	printInt(array[5], 10);
	newLine();
	print("RSI: ");
	printInt(array[6], 10);
	newLine();
	print("R8: ");
	printInt(array[7], 10);
	newLine();
	print("R9: ");
	printInt(array[8], 10);
	newLine();
	print("R10: ");
	printInt(array[9], 10);
	newLine();
	print("R11: ");
	printInt(array[10], 10);
	newLine();
	print("R12: ");
	printInt(array[11], 10);
	newLine();
	print("R13: ");
	printInt(array[12], 10);
	newLine();
	print("R14: ");
	printInt(array[13], 10);
	newLine();
	print("R15: ");
	printInt(array[14], 10);
	newLine();
}

void printMemContent(char* startPos, size_t amount){
	uint8_t memContent[amount];
	getMemContent(startPos, memContent, amount);
	for(int i=0 ; i < amount ; i++){
		print("BYTE ");
		printInt(startPos+i, 16);
		print(": ");
		printInt(memContent[i], 16);
		if((i+1)%4 == 0)
			newLine();
		else
			print("    ");
	}
}

// Prints daytime in format HH:MM:SS (24hs)
static void printTime(Time dayTime){
	char hours[3], minutes[3], seconds[3];
	numToStrSized(dayTime.hours, hours, 10, 2);
	numToStrSized(dayTime.minutes, minutes, 10, 2);
	numToStrSized(dayTime.seconds, seconds, 10, 2);
	print("Time: ");
	print(hours);
	print(":");
	print(minutes);
	print(":");
	println(seconds);
}

// Prints date in format DD/MM/YYYY
static void printDate(Date date){
	char day[3], month[3], year[5];
	numToStrSized(date.day, day, 10, 2);
	numToStrSized(date.month, month, 10, 2);
	numToStrSized(date.year, year, 10, 4);
	print("Date: ");
	print(day);
	print("/");
	print(month);
	print("/");
	println(year);
}

void printDateTime(int utc){
	Time currentTime;
	Date currentDate;
	int dateChanged = getTime(&currentTime, utc);
	getDateChanged(&currentDate, dateChanged);
	print("UTC: ");
	printInt(utc, 10);
	newLine();
	printDate(currentDate);
	printTime(currentTime);
}