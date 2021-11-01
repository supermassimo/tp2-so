#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stddef.h>
#include <console.h>
#include <featureStruct.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

int strcmp(char* s1, char* s2);
size_t strlen(char* string);
size_t totalStrlen(int dim, char* strArr[]);
void strcat(char* destination, const char* source);

size_t numToStr(long value, char* target, size_t base);
void numToStrSized(size_t value, char* target, size_t base, size_t length);
void getMemContent(uint64_t startPos, uint8_t* target, size_t count);
bool getCpuFeatures(CommonFeatures* commonFeat, ExtendedFeatures* extendedFeat);

char *cpuVendor(char *result);
uint32_t getCpuFeaturesEDXRaw();
uint32_t getCpuFeaturesECXRaw();
uint32_t getCpuExtendedFeaturesEBXRaw();
uint32_t getCpuExtendedFeaturesECXRaw();
int getCpuIdSupport();

#endif