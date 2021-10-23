#ifndef SYSCALLHANDLER_H_
#define SYSCALLHANDLER_H_

#include <lib.h>
#include <time.h>
#include <console.h>
#include <standardIn.h>
#include <keyboard.h>
#include <dualDisplayManager.h>
#include <stddef.h>
#include <rtc.h>
#include <scheduler.h>

void sysReadInput(char* buffer, size_t buffer_size);
void sysWrite(int output, const char* buffer, size_t buffer_size);
void sysGetMemContent(uint64_t startPos, uint8_t* memContent, size_t amount);
void sysGetDateTime(Date* date, Time* time, int utc);
void sysSetIdle(int isIdle);
void sysClear();
int sysGetActiveDisplay();
bool sysGetCpuFeatures(CommonFeatures* commonFeatures, ExtendedFeatures* extendedFeatures);
void sysSwapActiveDisplay();
void sysSleep(long seconds);
void sysDelKey();
void* sysMemAlloc(size_t size, int options);
int sysMemFree(void *blockp);
void sysGetMemInfo(MemoryInfo* meminfo);
int sysCreateProcess(void* entryPoint, Priority priority, int argc, char* argv[], char* name);
void sysKillCurrentProcess();

#endif