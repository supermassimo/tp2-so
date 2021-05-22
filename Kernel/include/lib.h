#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int numToStr(int value, char* target, uint8_t base);
void numToStrSized(int value, char* target, uint8_t base, int length);

char *cpuVendor(char *result);

#endif