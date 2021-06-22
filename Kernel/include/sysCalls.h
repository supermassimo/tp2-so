#ifndef SYSCALLHANDLER_H_
#define SYSCALLHANDLER_H_

#include <stddef.h>

void sysReadInput(char* buffer, size_t buffer_size);
void sysWrite(int output, const char* buffer, size_t buffer_size);
void sysGetMemContent(uint64_t startPos, uint8_t* memContent, size_t amount);
void sysWriteDateTime(int utc);
void sysSetIdle(int isIdle);
void sysClear();
int sysGetActiveDisplay();
void sysWriteCpuFeatures();
void sysSwapActiveDisplay();
void sysSleep(long seconds);
void sysDelKey();

#endif