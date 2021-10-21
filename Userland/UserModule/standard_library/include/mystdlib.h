#ifndef MYSTDLIB_H_
#define MYSTDLIB_H_

#define LOCAL_UTC -3

#include <stddef.h>
#include <stdint.h>
#include "commands.h"

/* memAlloc options*/
#define NO_ACTION   0
#define SET_ZERO    1

typedef enum UserPriority {LOW, MEDIUM, HIGH} UserPriority;

size_t numToStr(long value, char* target, size_t base);
void numToStrSized(long value, char* target, size_t base, size_t length);
long strToNum(char* string);
long strToNumPos(char* string);
int strcmp(char* s1, char* s2);
size_t strlen(char *string);
size_t concatStrings(char strings[][MAX_PARAMETER_LENGTH], size_t stringAmount, char* output);
size_t floatToStr(float value, char* target, size_t precision, size_t base);
int strToFloat(char* string, float* target);
void* malloc(size_t size);
void* calloc(size_t nmemb, size_t size);
int free(void* blockp);

#endif