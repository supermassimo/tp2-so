#include <stddef.h>
#include <stdint.h>
#include "commands.h"

size_t numToStr(size_t value, char* target, size_t base);
long strToNum(char* string);
size_t strToNumPos(char* string);
int strcmp(char* s1, char* s2);
size_t strlen(char *string);
int concatStrings(char strings[][MAX_PARAMETER_LENGTH], int stringAmount, char* output);