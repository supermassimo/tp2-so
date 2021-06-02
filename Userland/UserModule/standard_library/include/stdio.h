#ifndef STDIO_H_
#define STDIO_H_

#include <stddef.h>
#include <stdint.h>

void print (const char* string, size_t string_size);
void printInt (int num, size_t string_size, uint8_t base);
void scan (char* string, size_t string_size);

#endif

