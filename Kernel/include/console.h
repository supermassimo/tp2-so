#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

typedef enum Color {
	Black, Blue, Green, Cyan, Red, Purple, Brown, Gray, DarkGray, LightBlue, LightGreen, LightCyan, 
	LightRed, LightPurple, Yellow, White
} Color;

void setForeColor(uint8_t color);
void setBackColor(uint8_t color);
void print(char *string);
void printCol(char *string, uint8_t foreColor, uint8_t backColor);
void println(char *string);
void printlnCol(char *string, uint8_t foreColor, uint8_t backColor);
void printChar(char character);
void printCharCol(char character, uint8_t foreColor, uint8_t backColor);
void newLine();
void clearScreen();

#endif