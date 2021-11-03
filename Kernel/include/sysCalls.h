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
#include <semaphore.h>
#include <pipe.h>

void sysReadInput(int fd, char* buffer, size_t buffer_size);
void sysWrite(int output, const char* buffer, size_t buffer_size);
void sysGetMemContent(uint64_t startPos, uint8_t* memContent, size_t amount);
void sysGetDateTime(Date* date, Time* time, int utc);
void sysSetIdle(int isIdle);
void sysClear();
int sysGetActiveDisplay();
bool sysGetCpuFeatures(CommonFeatures* commonFeatures, ExtendedFeatures* extendedFeatures);
void sysSwapActiveDisplay();
int sysSleep(int pid, size_t seconds);
void sysDelKey();
void* sysMemAlloc(size_t size, int options);
int sysMemFree(void *blockp);
void sysGetMemInfo(MemoryInfo* meminfo);
int sysCreateProcess(void* entryPoint, Priority priority, int argc, char* argv[], char* name);
int sysNice(int pid, Priority priority);
int sysBlock(int pid, BlockOption option);
void sysExit(int status);
int sysKill(int pid, ProcessSignal sig);
void sysPrintAllProcesses();
int sysGetpid();
void sysSkip();
int sysSemOpen(char* sem_id, int value);
int sysSemDestroy(char* sem_id);
int sysSemWait(char* sem_id);
int sysSemPost(char* sem_id);
int sysSemSetValue(char* sem_id, int value);
int sysCreatePipe();
void sysClosePipe(int index);
int sysWritePipe(int index, const char* buf, int count);
int sysReadPipe(int index, char* buf, int count);
void sysPrintPipes();

#endif