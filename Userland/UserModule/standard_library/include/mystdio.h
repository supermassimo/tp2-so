#ifndef MYSTDIO_H_
#define MYSTDIO_H_

#include <stddef.h>
#include <stdint.h>

void printErr(const char* string);
void printf (const char* string);
void printInt (long num, size_t string_size, uint8_t base);
void scanf (char* string, size_t string_size);
void printFloat (float num, size_t string_size, size_t precision, uint8_t base);
void consoleSwap();

#endif

