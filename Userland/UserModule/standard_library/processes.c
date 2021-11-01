#include "./include/processes.h"

typedef enum ProcessSignal {SIG_KILL} ProcessSignal;
typedef enum BlockOption {BLOCK, UNBLOCK} BlockOption;

extern int _createProcess(void* entryPoint, UserPriority priority, int argc, char* argv[], char* name);
// extern void exit(int status);
extern int _kill(int pid, ProcessSignal sig);
extern int _nice(int pid, UserPriority priority);
extern int _block(int pid, BlockOption option);
extern void _printAllProcesses();
extern int _getpid();
extern void _skip();

int createProcess(void* entryPoint, char* name){
    return _createProcess(entryPoint, LOW, 0, NULL, name);
}

int createProcessWithParams(void* entryPoint, int argc, char* argv[], char* name){
    return _createProcess(entryPoint, LOW, argc, argv, name);
}

int createProcessWithPriority(void* entryPoint, UserPriority priority, char* name){
    return _createProcess(entryPoint, priority, 0, NULL, name);
}

int createFullProcess(void* entryPoint, UserPriority priority, int argc, char* argv[], char* name){
    return _createProcess(entryPoint, priority, argc, argv, name);
}

int killProcess(int pid){
    return _kill(pid, SIG_KILL);
}

int nice(int pid, UserPriority newPriority){
    return _nice(pid, newPriority);
}

int blockProcess(int pid){
    return _block(pid, BLOCK);
}

int unblockProcess(int pid){
    return _block(pid, UNBLOCK);
}

int getpid(){
    return _getpid();
}

void printAllProcesses(){
    _printAllProcesses();
}

void skipExecution(){
    _skip();
}