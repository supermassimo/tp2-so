#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>
#include <stddef.h>
#include <regStruct.h>

typedef enum Color {
	Black, Blue, Green, Cyan, Red, Purple, Brown, Gray, DarkGray, LightBlue, LightGreen, LightCyan, 
	LightRed, LightPurple, Yellow, White
} Color;

typedef struct ConsoleParameters {
    int cursorSymbol;
    int delimiterSymbol;
    Color leftDelimeterColor;
    Color rightDelimeterColor;
    Color leftBackgroundColor;
    Color rightBackgroundColor;
    Color leftActiveTextColor;
    Color rightActiveTextColor;
    Color leftInactiveTextColor;
    Color rightInactiveTextColor;
    Color leftActiveErrorColor;
    Color rightActiveErrorColor;
    Color leftInactiveErrorColor;
    Color rightInactiveErrorColor;
    int registryCaptureKey;
    int enterKey;
    int deleteKey;
} ConsoleParameters;

void initializeConsole(ConsoleParameters parameters);
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
void printRegistries(const Registries * regs);
void newLine();
void clearScreen();
void clearActiveConsole();
void deleteKeyConsole();
ConsoleParameters getParameters();

#endif