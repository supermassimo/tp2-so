#include <scheduler.h>

#define MAX_PROCESSES 10

typedef enum State {READY, BLOCKED, TERMINATED} State;

typedef struct {
    State state;
    uint64_t* pcb;
} Process;

extern uint64_t* createPCB(void* entryPoint);

static Process processes[MAX_PROCESSES] = {0};
static int activeProcessIdx = 0;
static size_t iterations = 0;
static size_t printsMade = 0;
static size_t printing = 0;

static void testProcess(){
    while(1){
        print("Print: ");
        printInt(printsMade++, 10);
        print("\n");
    }
}

void createProcess(){
    processes[0].pcb = createPCB(testProcess);
    printing = 1;
}

uint64_t* schedule(uint64_t* currentProcPCB){
    if(iterations % 2 == 0){
        processes[0].pcb = currentProcPCB;
        changeConsoleSide(1);
        print("\nCAMBIO con iterations:");
        printInt(iterations, 10);
        print(" en print:");
        printInt(printsMade, 10);
        print(" ");
        printInt(printing, 10);
        print("\n");
        changeConsoleSide(0);
    }
    iterations++;
    return processes[0].pcb;
}