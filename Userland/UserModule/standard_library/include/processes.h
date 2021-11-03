#ifndef PROCESSES_H
#define PROCESSES_H

#include <stddef.h>

typedef enum UserPriority {LOW, MEDIUM, HIGH} UserPriority;

int createProcess(void* entryPoint, char* name);
int createProcessWithParams(void* entryPoint, int argc, char* argv[], char* name);
int createProcessWithPriority(void* entryPoint, UserPriority priority, char* name);
int createFullProcess(void* entryPoint, UserPriority priority, int argc, char* argv[], char* name);
int killProcess(int pid);
int nice(int pid, UserPriority newPriority);
int blockProcess(int pid);
int unblockProcess(int pid);
int getpid();
void printAllProcesses();
void skipExecution();

#endif