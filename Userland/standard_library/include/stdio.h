#ifndef STDIO_H_
#define STDIO_H_

#include <stdef.h>

void printf (const char* string, size_t string_size);
void printInt (int num, size_t string_size, uint8_t base);
void scanf (char* string, size_t string_size);

#endif

