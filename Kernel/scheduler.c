#include <scheduler.h>

#define MAX_PROCESSES  10
#define PCB_REGISTERS  21

extern uint64_t* createPCB(uint64_t* wrapper, uint64_t* pcbAddr, uint64_t* entryPoint, int argc, char* argv[]);
extern void scheduleNext();

static Process processes[MAX_PROCESSES] = {0};
static int currentProcess = -1;
static int isSchedulerEnabled = 0;
static int activeProcesses = 0;
static size_t currentProcessQuantums = 0;
static int outsideRtc = 0;

void enableScheduler(){
    isSchedulerEnabled = 1;
}

char* getStateString(State state){
    switch(state){
        case 0:
            return "T";
        case 1:
            return "R";
        case 2:
            return "B";
        default:
            return "?";
    }
}

static int getFirstFree(){
    for(int i=0 ; i < MAX_PROCESSES ; i++){
        if(processes[i].state == TERMINATED)
            return i;
    }
    return -1;
}

static void* loadArgv(int argc, char* argv[]){
    size_t argvLen = totalStrlen(argc, argv);
    void* argPtr = memAlloc(argvLen, NO_ACTION);
    memcpy(argPtr, argv, argvLen);
    return argPtr;
}

static void processWrapper(void* processEntry, int argc, char* argv[]){
    int status = ((int(*)(int, char*[]))processEntry)(argc, argv);
    exit(status);
}

int createProcess(void* entryPoint, Priority priority, int argc, char* argv[], char* name){
    int processIdx = getFirstFree();
    if(processIdx == -1)
        return processIdx;
    uint64_t* baseAddr = memAlloc(sizeof(uint64_t) * PROCESS_STACK, SET_ZERO);
    processes[processIdx].argv = loadArgv(argc, argv);
    processes[processIdx].pcb = createPCB(processWrapper, baseAddr+(PROCESS_STACK-2), entryPoint, argc, processes[processIdx].argv);
    processes[processIdx].base = baseAddr;
    processes[processIdx].state = READY;
    processes[processIdx].priority = priority;
    processes[processIdx].name = name;
    processes[processIdx].startTime = seconds_elapsed();
    activeProcesses++;
    return processIdx;
}

int getpid(){
    return currentProcess;
}

void printAllProcesses(){
    printProcesses(processes, activeProcesses);
}

int nice(int pid, Priority priority){
    if(priority == SYSTEM || processes[pid].state == TERMINATED)
        return -1;
    processes[pid].priority = priority;
    return 0;
}

int block(int pid){
    if(processes[pid].state == TERMINATED)      // Might be useless. Remove if no more states are added (apart from READY/BLOCKED/TERMINATED)
        return -1;
    switch(processes[pid].state){
        case READY:
            processes[pid].state = BLOCKED;
            return 0;
        case BLOCKED:
            processes[pid].state = READY;
            return 1;
        default:        // Process was terminated
            return -1;
    }
}

void skip(){
    outsideRtc = 1;
    scheduleNext();
}

int scheduleOutsideRtc(){
    if(outsideRtc == 1){
        outsideRtc = 0;
        return 1;
    }
    return 0;
}

void exit(int status){
    processes[currentProcess].state = TERMINATED;
    outsideRtc = 1;
    scheduleNext();
}

static int killProcess(int pid){
    if(processes[pid].state == TERMINATED)
        return -1;
    processes[pid].state = TERMINATED;
    return 0;
}

int kill(int pid, ProcessSignal sig){
    switch(sig){
        case 0:
            return killProcess(pid);
        default:
            return -1;
    }
}

void printProcess(uint64_t* currentProcPCB) {
    changeConsoleSide(1);
    print("PCB ACTUAL:\n");
    for(int i=0 ; i < 21 ; i++){
        printInt(currentProcPCB+i, 16);
        print("\n");
    }
    print("\n");
    print("\n");
    print("\n");
    changeConsoleSide(0);
}

static int getNextReady(int current){
    while(processes[current+1].state != READY){
        current++;
        if(current+1 == MAX_PROCESSES)
            current = -1;
    }
    return current+1;
}

uint64_t* schedule(uint64_t* currentProcPCB){
    if(isSchedulerEnabled){
        if(currentProcess == -1){       // Only when no processes are running yet (only kernel)
            currentProcessQuantums++;
            return processes[++currentProcess].pcb;
        }
        if(processes[currentProcess].state != TERMINATED){
            processes[currentProcess].pcb = currentProcPCB;
        }
        if(processes[currentProcess].state == TERMINATED || processes[currentProcess].priority < currentProcessQuantums){
            if(processes[currentProcess].state == TERMINATED){
                memFree(processes[currentProcess].base);
                memFree(processes[currentProcess].argv);
                activeProcesses--;
            }
            currentProcessQuantums = 0;
            currentProcess = getNextReady(currentProcess);
        }
        currentProcPCB = processes[currentProcess].pcb;
        currentProcessQuantums++;
        // printProcess(processes[currentProcess].pcb);
    }
    return currentProcPCB;
}

