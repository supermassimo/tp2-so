#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <registryStruct.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int numToStr(int value, char* target, uint8_t base);
void numToStrSized(int value, char* target, uint8_t base, int length);

char *cpuVendor(char *result);

int64_t getRAX ();
int64_t getRBX ();
int64_t getRCX ();
int64_t getRDX ();
int64_t getRBP ();
int64_t getRDI ();
int64_t getRSI ();
int64_t getR8 ();
int64_t getR9 ();
int64_t getR10 ();
int64_t getR11 ();
int64_t getR12 ();
int64_t getR13 ();
int64_t getR14 ();
int64_t getR15 ();

void getRegistries (Registries* destination);

#endif