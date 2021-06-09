#ifndef MYSTDLIB_H_
#define MYSTDLIB_H_

#define LOCAL_UTC -3

#include <stddef.h>
#include <stdint.h>
#include "commands.h"

size_t numToStr(size_t value, char* target, size_t base);
long strToNum(char* string);
long strToNumPos(char* string);
int strcmp(char* s1, char* s2);
size_t strlen(char *string);
size_t concatStrings(char strings[][MAX_PARAMETER_LENGTH], size_t stringAmount, char* output);
size_t floatToStr(float value, char* target, size_t precision);
void strToFloat(char* string, float* target);

#endif