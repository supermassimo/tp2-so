#include <console.h>
#include <lib.h>
// #include <commands.h>

#define SCR_BASE_ADDR 	0xB8000
#define SCR_ROWS 		25
#define SCR_COLS 		80

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

void printInt(int num, uint8_t base){
	printIntCol(num, base, foreColor, backColor);
}

void printIntCol(int num, uint8_t base, uint8_t foreColor, uint8_t backColor){
	char strNum[12];
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
