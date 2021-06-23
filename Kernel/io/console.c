#include <console.h>
#include <lib.h>
#include <rtc.h>
#include <standardIn.h>

#define SCR_BASE_ADDR 	0xB8000
#define SCR_ROWS 		25
#define SCR_COLS 		80
#define SCR_SIDE_COLS	39
#define LIMITER_GIRTH	2

static ConsoleParameters parameters  = {	// Default parameters
		'>','|',
		Green,Blue,Black,Black,White,White,DarkGray,DarkGray,Red,Red,Brown,Brown,
		'~','\n','\b'
	};

static char* scrPos = SCR_BASE_ADDR; 		// Current position on the screen

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
	uint8_t delimiterColor;
} Console;

/* 
--------||--------
--------||--------
--------||--------
--------||--------
--------||--------
*/

static const int consoleAmount = 2;

static Console consoles[2];

static void drawDelimiterInRow(size_t col, size_t row, size_t girth, uint8_t colorByte){
	char* delim = SCR_BASE_ADDR + col*2 + row*SCR_COLS*2 - 2;
	for (int x=0; x<girth; x++){
		*(delim+(x*2)) = parameters.delimiterSymbol;
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
	drawDelimiter(SCR_SIDE_COLS+1, LIMITER_GIRTH, consoles[targetConsole].delimiterColor);
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

void deleteKeyConsole(){
	char* finalPos;
	if(scrPos-2 >= consoles[activeConsole].startPos){			// Check if there are chars to delete
		finalPos = getPrevCharAddr(activeConsole);
		*finalPos = *(finalPos+2);								// Backspace functionality
		*(finalPos+1) = getColorByte(Black, Black);				// Fill current char with next one and move cursor one place back
		scrPos = finalPos;
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
	printCharCol(c, consoles[activeConsole].activeForeColor, consoles[activeConsole].backColor);
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
	if(c == '\n'){ //not the "enterKey" parameter, specifically new line
		newLine();
	}
    else{
        *scrPos = c;
        *(scrPos+1) = colorByte;
        scrPos += 2;
    }
}

void printCol(const char* msg, uint8_t foreColor, uint8_t backColor){
	for(int i=0 ; msg[i] != 0 ; i++){
		printCharCol(msg[i], foreColor, backColor);
	}
}

void print(const char* msg){
	printCol(msg, consoles[activeConsole].activeForeColor, consoles[activeConsole].backColor);
}

void printlnCol(const char* msg, uint8_t foreCol, uint8_t backCol){
	printCol(msg, foreCol, backCol);
	newLine();
}

void println(const char* msg){
	printlnCol(msg, consoles[activeConsole].activeForeColor, consoles[activeConsole].backColor);
}

void printInt(long num, size_t base){
	printIntCol(num, base, consoles[activeConsole].activeForeColor, consoles[activeConsole].backColor);
}

void printIntCol(long num, size_t base, uint8_t foreCol, uint8_t backCol){
	char strNum[BUFFER_SIZE];
	numToStr(num, strNum, base);
	printCol(strNum, foreCol, backCol);
}

void printErr(const char* msg){
	printlnCol(msg, consoles[activeConsole].errorColor, consoles[activeConsole].backColor);
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

static void printRegistry(const char* msg, uint64_t value){
	print(msg);
	printInt(value, 16);
	print("\n");
}

void printRegistries(const Registries * regs){
	printRegistry("RAX: ", regs -> RAX);
	printRegistry("RBX: ", regs -> RBX);
	printRegistry("RCX: ", regs -> RCX);
	printRegistry("RDX: ", regs -> RDX);
	printRegistry("RDI: ", regs -> RDI);
	printRegistry("RSI: ", regs -> RSI);
	printRegistry("R8: " , regs -> R8 );
	printRegistry("R9: " , regs -> R9 );
	printRegistry("R10: ", regs -> R10);
	printRegistry("R11: ", regs -> R11);
	printRegistry("R12: ", regs -> R12);
	printRegistry("R13: ", regs -> R13);
	printRegistry("R14: ", regs -> R14);
	printRegistry("R15: ", regs -> R15);
	printRegistry("RBP: ", regs -> RBP);
	printRegistry("RSP: ", regs -> RSP);
	printRegistry("RIP: ", regs -> RIP);
}

static int wasInitialized = 0;

void initializeConsole (ConsoleParameters params){
	if (wasInitialized == 1) return;
	parameters = params;

	Console cons0 = {(const char*)SCR_BASE_ADDR, (const char*)SCR_BASE_ADDR, SCR_ROWS, SCR_SIDE_COLS, parameters.leftActiveTextColor, parameters.leftInactiveTextColor, parameters.leftBackgroundColor, parameters.leftInactiveTextColor, parameters.leftActiveErrorColor, parameters.leftDelimeterColor};
	Console cons1 = {(const char*)SCR_BASE_ADDR+(SCR_SIDE_COLS+LIMITER_GIRTH)*2, (const char*)SCR_BASE_ADDR+(SCR_SIDE_COLS+LIMITER_GIRTH)*2, SCR_ROWS, SCR_SIDE_COLS, parameters.rightActiveTextColor, parameters.rightInactiveTextColor, parameters.rightBackgroundColor, parameters.rightInactiveTextColor, parameters.rightActiveErrorColor, parameters.rightDelimeterColor};

	consoles[0] = cons0;
	consoles[1] = cons1;
	
	drawDelimiter(SCR_SIDE_COLS+1, LIMITER_GIRTH, parameters.leftDelimeterColor);

	char cursor[] = "  ";
	cursor[0] = parameters.cursorSymbol;
	print(cursor);
	swapDisplay();
	print(cursor);
	swapDisplay();

	wasInitialized = 1;
}