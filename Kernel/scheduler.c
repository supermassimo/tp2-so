// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <scheduler.h>

#define PCB_REGISTERS   21
#define SHELL_PID       1

extern uint64_t* createPCB(uint64_t* wrapper, uint64_t* pcbAddr, int argc, char* argv[], void* entryPoint);
extern void _hlt();
extern void scheduleNext();

static Process processes[MAX_PROCESSES] = {{0}};
static int currentProcess = -1;
static int isSchedulerEnabled = 0;
static int activeProcesses = 0;
static size_t currentProcessQuantums = 0;
static int outsideRtc = 0;

static int haltShell(int argc, char* argv[]){
    while(1){
        _hlt();
    }
    return 0;
}

void printProcesses(Process* processes, size_t amount){
	size_t printed = 0;
	size_t length = 0;
	print("PID CMD      P  S  F  BP     SP\n");
	for(int i=0 ; printed < amount ; i++){
		if(processes[i].state != TERMINATED){
			printInt(i, 10);
			print("   ");
			print(processes[i].name);
			length = strlen(processes[i].name);
			for(int j=length; j<9; j++) {
				print(" ");
			}
			printInt(processes[i].priority, 10);
			print("  ");
			print(getStateString(processes[i].state));
			print("  ");
			print(i == 0 ? "Y" : "N");
			print("  ");
			printInt((size_t)processes[i].base, 16);
			print(" ");
			printInt((size_t)processes[i].pcb, 16);
			print("\n");
			printed++;
		}
	}
}

void createHaltingProcess(){
    createProcess(haltShell, LOW, 0, NULL, "hlt");
}

void enableScheduler(){
    isSchedulerEnabled = 1;
}

char* getStateString(State state){
    switch(state){
        case TERMINATED:
            return "T";
        case READY:
            return "R";
        case BLOCKED:
            return "B";
        case SLEEP:
            return "S";
        case WAITING:
            return "W";
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
    void* argPtr = memAlloc(argvLen+argc*2+1+argc, SET_ZERO);
    memcpy(argPtr, argv, argvLen+argc*2+1+argc);
    return argPtr;
}

void processWrapper(int argc, char* argv[], void* processEntry){
    int status = ((int(*)(int, char*[]))processEntry)(argc, argv);
    exit(status);
}

int createProcess(void* entryPoint, Priority priority, int argc, char* argv[], char* name){
    int processIdx = getFirstFree();
    if(processIdx == -1)
        return processIdx;
    uint64_t* baseAddr = memAlloc(sizeof(uint64_t) * PROCESS_STACK, SET_ZERO);
    processes[processIdx].argv = loadArgv(argc, argv);
    processes[processIdx].pcb = createPCB((uint64_t*)processWrapper, baseAddr+(PROCESS_STACK-2), argc, processes[processIdx].argv, entryPoint);
    processes[processIdx].base = baseAddr;
    processes[processIdx].priority = priority;
    processes[processIdx].name = name;
    activeProcesses++;
    processes[processIdx].state = READY;
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

int block(int pid, BlockOption option){
    if(processes[pid].state == TERMINATED)
        return -1;
    switch(option){
        case BLOCK:
            if(processes[pid].state == BLOCKED)
                return -1;
            processes[pid].state = BLOCKED;
            break;
        case UNBLOCK:
            if(processes[pid].state != BLOCKED)
                return -1;
            processes[pid].state = READY;
            break;
        default:            // Any other case
            return -1;
    }
    return 0;
}

int sleep(int pid, size_t seconds){
    if(processes[pid].state == TERMINATED){
        return -1;
    }
    processes[pid].sleepTime = seconds + seconds_elapsed();
    processes[pid].state = SLEEP;
    outsideRtc = 1;
    scheduleNext();
    return 0;
}

//parecido a sleep
//deja al proceso en espera por un tiempo indefinido, hasta wakeup()
int makeWait(int pid){
    if(processes[pid].state == TERMINATED){
        return -1;
    }
    processes[pid].state = WAITING;
    outsideRtc = 1;
    scheduleNext();
    return 0;
}

//despierta un proceso que este en espera o en sleep
int wakeup(int pid){
    if(processes[pid].state == TERMINATED){
        return -1;
    }
    if(processes[pid].state == WAITING || processes[pid].state == SLEEP){
        processes[pid].state = READY;
    }
    return 0;
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
    processes[currentProcess].state = TO_TERMINATE;
    outsideRtc = 1;
    scheduleNext();
}

static int killProcess(int pid){
    if(processes[pid].state == TERMINATED)
        return -1;
    processes[pid].state = TO_TERMINATE;
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

static int getNextReady(int current){
    if(processes[current].state == TO_TERMINATE){             // Process garbage collector
        processes[current].state = TERMINATED;
        memFree(processes[current].base);
        memFree(processes[current].argv);
        activeProcesses--;
    }
    while(processes[current+1].state != READY){
        current++;
        if(processes[current].state == TO_TERMINATE){             // Process garbage collector
            processes[current].state = TERMINATED;
            memFree(processes[current].base);
            memFree(processes[current].argv);
            activeProcesses--;
        }
        if(processes[current].state == SLEEP && processes[current].sleepTime <= seconds_elapsed()){
            processes[current].state = READY;
            current--;
        }
        if(current+1 == MAX_PROCESSES)
            current = 0;
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
        if(processes[currentProcess].state != READY || processes[currentProcess].priority < currentProcessQuantums){
            if(processes[currentProcess].pcb < processes[currentProcess].base){         // If process' stack exceeded reserved space, kill it
                kill(currentProcess, SIG_KILL);
            }
            currentProcessQuantums = 0;
            if(activeProcesses == 2 && currentProcess == SHELL_PID && keyboardBufferIsEmpty()){
                currentProcess = 0;
            }
            else {
                currentProcess = getNextReady(currentProcess);
            }
        }
        currentProcPCB = processes[currentProcess].pcb;
        currentProcessQuantums++;
    }
    return currentProcPCB;
}

