#ifndef COMMANDS_H_
#define COMMANDS_H_

#define MAX_COMMAND_LENGTH 120
#define MAX_PARAMETER_AMOUNT 10
#define MAX_PARAMETER_LENGTH 120

#include "mystdio.h"
#include "mystdlib.h"
#include "processes.h"
#include "semaphore.h"

enum CommandState {COMMAND1, PARAM1, COMMAND2, PARAM2, PIPED};

void commandHandler(char* string);

extern void getRegistries(Registries* regs);
extern void getMemContent(uint64_t startPos, uint8_t* target, size_t amount);
extern void invalidOpcodeThrower();
extern void setIdle(int idle);
extern void clear();
extern bool getCpuFeatures(CommonFeatures* commonFeatures, ExtendedFeatures* extendedFeatures);
extern int sleep(int pid, size_t seconds);
extern int getQuadratic(float a, float b, float c, float*, float*);
extern void getDateTime(Date* date, Time* time, int utc);
extern void getMemInfo(MemoryInfo* meminfo);
extern int createPipe();
extern void closePipe(int id);
extern int writePipe(int id, const char *buf, int count);
extern int readPipe(int id, char *buf, int count);
extern void printPipes();

#endif