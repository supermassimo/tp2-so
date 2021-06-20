#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>
#include <stddef.h>

typedef enum Color {
	Black, Blue, Green, Cyan, Red, Purple, Brown, Gray, DarkGray, LightBlue, LightGreen, LightCyan, 
	LightRed, LightPurple, Yellow, White
} Color;

void initializeConsole();
void changeConsoleSide(int targetConsole);
void setActiveForeColor(int consoleIdx, Color foreColor);
void setInactiveForeColor(int consoleIdx, Color foreColor);
void setBackColor(int consoleIdx, Color backColor);
void setErrorColor(int consoleIdx, Color errorColor);
void print(const char *string);
void printCol(const char *string, uint8_t foreColor, uint8_t backColor);
void println(const char *string);
void printlnCol(const char *string, uint8_t foreColor, uint8_t backColor);
void printChar(const char character);
void printCharCol(const char character, uint8_t foreColor, uint8_t backColor);
void printInt(long num, size_t base);
void printIntCol(long num, size_t base, uint8_t foreColor, uint8_t backColor);
void printErr(const char *string);
void newLine();
void clearScreen();
void clearActiveConsole();
void printRegistries();
void printMemContent(char* startPos, size_t amount);
void printDateTime(int utc);
void deleteKeyConsole();

#endif