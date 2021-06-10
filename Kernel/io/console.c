#include <console.h>
#include <lib.h>
#include <rtc.h>

#define SCR_BASE_ADDR 	0xB8000
#define SCR_ROWS 		25
#define SCR_COLS 		80
#define SCR_SIDE_COLS	39
#define LIMITER_GIRTH	2

#define MAX_NUM_LENGTH 255					// For printing numbers purposes

static char* scrPos = SCR_BASE_ADDR; 		// Current position on the screen
static uint8_t foreColor = White;			// Default Forecolor
static uint8_t backColor = Black;			// Default Backcolor
static uint8_t errorColor = Red;

static uint8_t delimiterColor = Green;

#define CONSOLE_0_START SCR_BASE_ADDR
#define CONSOLE_1_START SCR_BASE_ADDR+(SCR_SIDE_COLS+LIMITER_GIRTH)*2

static int activeConsole = 0;

typedef struct Console
{
	const char* startPos;
	char* currentPos;
	const size_t rows;
	const size_t cols;
	uint8_t activeForeColor;
	uint8_t inactiveForeColor;
	uint8_t backColor;
	uint8_t inactiveErrorColor;
	uint8_t errorColor;
} Console;

/* 
--------||--------
--------||--------
--------||--------
--------||--------
--------||--------
*/

static Console consoles[] = {
	{CONSOLE_0_START, CONSOLE_0_START, SCR_ROWS, SCR_SIDE_COLS, White, DarkGray, Black, Brown, Red},
	{CONSOLE_1_START, CONSOLE_1_START, SCR_ROWS, SCR_SIDE_COLS, White, DarkGray, Black, Brown, Red}
};

static const int consoleAmount = 2;

static void drawDelimiterInRow(size_t col, size_t row, size_t girth, uint8_t colorByte){
	char* delim = SCR_BASE_ADDR + col*2 + row*SCR_COLS*2 - 2;
	for (int x=0; x<girth; x++){
		*(delim+(x*2)) = '|';
        *(delim+1+(x*2)) = colorByte;
	}
		
}

static void drawDelimiter(size_t col, size_t girth, uint8_t colorByte){
	for(int y=0 ; y < SCR_ROWS ; y++){
		drawDelimiterInRow(col, y, girth, colorByte);
	}
}

static uint8_t getColorByte(uint8_t foreColor, uint8_t backColor){
	return backColor << 4 | foreColor;
}

static uint8_t getColorForFillScreen(int consoleIdx, uint8_t thisColor, int enabled){
	if (enabled){
		if (thisColor == consoles[consoleIdx].inactiveForeColor) return consoles[consoleIdx].activeForeColor;
		if (thisColor == consoles[consoleIdx].inactiveErrorColor) return consoles[consoleIdx].errorColor;
	} else {
		if (thisColor == consoles[consoleIdx].activeForeColor) return consoles[consoleIdx].inactiveForeColor;
		if (thisColor == consoles[consoleIdx].errorColor) return consoles[consoleIdx].inactiveErrorColor;
	}
	return thisColor;
}

static void fillScreen(int consoleIdx, uint8_t backColor, int enabled){
	char *current = consoles[consoleIdx].startPos;
	size_t extraSpaces = (SCR_SIDE_COLS + LIMITER_GIRTH) * (consoleAmount-1);
	for(int i=1 ; i <= consoles[consoleIdx].rows ; i++){
		for(int j=1 ; j < consoles[consoleIdx].cols*2 ; j+=2){
			*(current+j) = getColorByte(getColorForFillScreen(consoleIdx, *(current+j), enabled), backColor);
		}
		current = consoles[consoleIdx].startPos + SCR_COLS*2*i;
	}
}

static int isActive(int consoleIdx){
	return consoleIdx == activeConsole;
}

static void setActive(int consoleIdx){
	fillScreen(consoleIdx, consoles[consoleIdx].backColor, 1);
}

static void setInactive(int consoleIdx){
	fillScreen(consoleIdx, consoles[consoleIdx].backColor, 0);
}

void changeConsoleSide(int targetConsole){
	consoles[activeConsole].currentPos = scrPos;
	scrPos = consoles[targetConsole].currentPos;
	activeConsole = targetConsole;
	for(int i=0 ; i < consoleAmount; i++){
		if(i == activeConsole)
			setActive(i);
		else
			setInactive(i);
	}
	switch (targetConsole){
		case 0:
			delimiterColor = Green;
			break;
		case 1:
			delimiterColor = Blue;
			break;
	}
	drawDelimiter(SCR_SIDE_COLS+1, LIMITER_GIRTH, delimiterColor);
}

void newLine(){
	int current = scrPos - SCR_BASE_ADDR;
	current /= SCR_COLS * 2;
	scrPos = consoles[activeConsole].startPos + (current+1) * SCR_COLS * 2;
}

void setActiveErrorColor(int consoleIdx, Color foreColor){
	consoles[consoleIdx].errorColor = foreColor;
	if(isActive(consoleIdx))
		setActive(consoleIdx);
}

void setInactiveErrorColor(int consoleIdx, Color foreColor){
	consoles[consoleIdx].inactiveErrorColor = foreColor;
	if(!isActive(consoleIdx))
		setInactive(consoleIdx);
}

void setActiveForeColor(int consoleIdx, Color foreColor){
	consoles[consoleIdx].activeForeColor = foreColor;
	if(isActive(consoleIdx))
		setActive(consoleIdx);
}

void setInactiveForeColor(int consoleIdx, Color foreColor){
	consoles[consoleIdx].inactiveForeColor = foreColor;
	if(!isActive(consoleIdx))
		setInactive(consoleIdx);
}

void setBackColor(int consoleIdx, Color backColor){
	consoles[consoleIdx].backColor = backColor;
	fillScreen(consoleIdx, consoles[consoleIdx].backColor, isActive(consoleIdx));
}

void setErrorColor(int consoleIdx, Color errorColor){
	consoles[consoleIdx].errorColor = errorColor;
}

// Returns the previous char address omitting 'empty' chars
static uint64_t getPrevCharAddr(int consoleIdx){
	char *prevChar = scrPos;
	long extraSpaces = (SCR_SIDE_COLS + LIMITER_GIRTH + 1) * (consoleAmount-1) * 2;
	do{
		if((prevChar - consoles[consoleIdx].startPos)%(SCR_COLS*2) == 0)
			prevChar -= extraSpaces;
		else
			prevChar -= 2;
	}while(*prevChar == 0);
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
			if(scrPos-2 >= consoles[activeConsole].startPos){			// Check if there are chars to delete
				finalPos = getPrevCharAddr(activeConsole);
				*finalPos = *(finalPos+2);			// Backspace functionality
				*(finalPos+1) = getColorByte(Black, Black);		// Fill current char with next one and move cursor one place back
				scrPos = finalPos;
			}
			break;
		default:
			break;
	}
}

static void scrollUpActiveConsole(){
	char* current = consoles[activeConsole].startPos;
	// Muevo todos los caracteres  de la consola activa una posicion hacia arriba
	for(int i=1 ; i < consoles[activeConsole].rows ; i++){
		for(int j=0 ; j < consoles[activeConsole].cols ; j++){
			*current = *(current+SCR_COLS*2);
			*(current+1) = *(current+SCR_COLS*2+1);
			current += 2;
		}
		current = consoles[activeConsole].startPos + SCR_COLS*2*i;
	}
	// Limpio la última línea, dejando el color previamente usado
	for(int j=0 ; j < consoles[activeConsole].cols ; j++){
		*current = 0;
		current += 2;
	}
	// Resetting the cursor to first char of last row
	scrPos = consoles[activeConsole].startPos + (consoles[activeConsole].rows-1)*SCR_COLS*2;
}

void printChar(char c){
	printCharCol(c, foreColor, backColor);
}

int isLastConsoleActive(){
	return activeConsole == consoleAmount - 1;
}

void printCharCol(char c, uint8_t foreColor, uint8_t backColor){
    char colorByte = getColorByte(foreColor, backColor);
	uint64_t offset = (uint64_t)scrPos-(uint64_t)consoles[activeConsole].startPos;
	size_t absoluteColumn = offset%(SCR_COLS*2);
	if(absoluteColumn % consoles[activeConsole].cols == 0 && absoluteColumn != 0){
		if(!isLastConsoleActive())
			newLine();
		else{
			size_t prevRow = offset / (SCR_COLS*2);
			scrPos = consoles[activeConsole].startPos + SCR_COLS*2*(prevRow+1);
		}
	}
	if(scrPos >= SCR_BASE_ADDR + SCR_ROWS * SCR_COLS * 2){
        scrollUpActiveConsole();
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

void clearActiveConsole(){
	char *current = consoles[activeConsole].startPos;
	for(int i=1 ; i <= consoles[activeConsole].rows ; i++){
		for(int j=0 ; j < consoles[activeConsole].cols ; j++){
			*current = 0;
			*(current+1) = 0;
			current += 2;
		}
		current = consoles[activeConsole].startPos + SCR_COLS*2*i;
	}
	scrPos = consoles[activeConsole].startPos;
}

void printRegistries(){
	uint64_t array[16];
	getRegistries(array, 16);
	print("RIP: ");
	printInt(array[0], 10);
	newLine();
	print("RAX: ");
	printInt(array[1], 10);
	newLine();
	print("RBX: ");
	printInt(array[2], 10);
	newLine();
	print("RCX: ");
	printInt(array[3], 10);
	newLine();
	print("RDX: ");
	printInt(array[4], 10);
	newLine();
	print("RBP: ");
	printInt(array[5], 10);
	newLine();
	print("RDI: ");
	printInt(array[6], 10);
	newLine();
	print("RSI: ");
	printInt(array[7], 10);
	newLine();
	print("R8: ");
	printInt(array[8], 10);
	newLine();
	print("R9: ");
	printInt(array[9], 10);
	newLine();
	print("R10: ");
	printInt(array[10], 10);
	newLine();
	print("R11: ");
	printInt(array[11], 10);
	newLine();
	print("R12: ");
	printInt(array[12], 10);
	newLine();
	print("R13: ");
	printInt(array[13], 10);
	newLine();
	print("R14: ");
	printInt(array[14], 10);
	newLine();
	print("R15: ");
	printInt(array[15], 10);
	newLine();
}

void printMemContent(char* startPos, size_t amount){
	uint8_t memContent[amount];
	char numStr[3];
	getMemContent(startPos, memContent, amount);
	for(int i=0 ; i < amount ; i++){
		if(i%8 == 0){
			printInt(startPos+i, 16);
			print("h: ");
		}
		numToStrSized((size_t)memContent[i], numStr, 16, 2);
		print(numStr);
		print(" ");
		if((i+1)%8 == 0)
			newLine();
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

void initializeConsole (){
	drawDelimiter(SCR_SIDE_COLS+1, LIMITER_GIRTH, delimiterColor);
}