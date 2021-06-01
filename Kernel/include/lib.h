#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include <stddef.h>
#include <registryStruct.h>
#include <console.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int numToStr(size_t value, char* target, uint8_t base);
void numToStrSized(size_t value, char* target, uint8_t base, int length);
void getMemContent(uint64_t startPos, uint8_t* target, size_t count);

char *cpuVendor(char *result);
uint32_t getCpuFeaturesEDXRaw();
uint32_t getCpuFeaturesECXRaw();
uint32_t getCpuExtendedFeaturesEBXRaw();
uint32_t getCpuExtendedFeaturesECXRaw();
void printCpuFeatures();

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

void printRegistries();
void getRegistries (Registries* destination);

#endif