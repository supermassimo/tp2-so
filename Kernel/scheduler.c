#include <scheduler.h>
#include <kernel.h>

#define MAX_PROCESSES 10

typedef enum State {READY, BLOCKED, SUSPENDED, TERMINATED} State;

typedef struct {
    uint64_t* pcb;
} Process;

extern uint64_t* createPCB(uint64_t* entryPoint, uint64_t pcbAddr);

static Process processes[MAX_PROCESSES] = {0};
static int activeProcessIdx = 0;
static size_t iterations = 0;
static size_t printsMade = 0;
static int wasCreated = 0;

void testProcess(){
    while(1){
        print("Print: ");
        printInt(printsMade++, 10);
        print("\n");
    }
}

void createProcess(){
    uint64_t* pcbAddr = memAlloc(sizeof(uint64_t) * 21, NO_ACTION);
    processes[0].pcb = createPCB(testProcess, pcbAddr);
    /*
    print("CREANDO PCB\n");
    for(int i=0 ; i < 21 ; i++){
        printInt(processes[0].pcb[i], 16);
        print("\n");
    }
    wasCreated = 1;
    */
}

uint64_t* schedule(uint64_t* currentProcPCB){
    if(isSchedulerEnabled()){
        changeConsoleSide(1);
        print("PCB ACTUAL: ");
        printInt(iterations, 10);
        print(" iterations\n");
        for(int i=0 ; i < 21 ; i++){
            printInt(processes[0].pcb[i], 16);
            print("\n");
        }
        print("\n");
        print("\n");
        print("\n");
        changeConsoleSide(0);
        iterations++;
        return processes[0].pcb;
    }
    return currentProcPCB;
}