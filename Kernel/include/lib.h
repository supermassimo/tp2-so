#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <registryStruct.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int numToStr(int value, char* target, uint8_t base);
void numToStrSized(int value, char* target, uint8_t base, int length);
void getMemContent(char* startPos, char* target, unsigned count);

char *cpuVendor(char *result);

void* getRAX ();
void* getRBX ();
void* getRCX ();
void* getRDX ();
void* getRBP ();
void* getRDI ();
void* getRSI ();
void* getR8 ();
void* getR9 ();
void* getR10 ();
void* getR11 ();
void* getR12 ();
void* getR13 ();
void* getR14 ();
void* getR15 ();

void getRegistries (Registries* destination);

#endif