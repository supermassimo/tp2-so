// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/commands.h"
#include "./include/mystdio.h"
#include "./include/mystdlib.h"
#include "./include/processes.h"

/*              TEST MM HEADERS             */
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include "./include/test_util.h"
/*******************************************/

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
extern void printAllSemaphores();
extern void printPipes();
extern void *getSharedMem(int id);

#define PRINTMEM_BYTES 32

typedef enum CommandType {BUILT_IN, PROCESS} CommandType;

typedef struct commandStruct{
    char* name;
    void* handler;
    char* help_message;
    CommandType type;
} commandStruct;

typedef struct testProgramStruct{
    char* name;
    void* entryPoint;
} testProgramStruct;

static const size_t exceptionAmount = 2;

#define QUADRATIC_PRECISION 2
#define FLOAT_STRING_SIZE 100

static void echoHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 3){
        printErr("Missing parameter for command 'echo'");
        return;
    }
    size_t finalLength = 0, actualLength;
    for(int i=0 ; i < paramAmount-2 ; i++){
        finalLength += strlen(params[i]) + 1;
    }
    char output[finalLength + 1];
    actualLength = concatStrings(params, paramAmount-2, output);
    printf(output);
    printf("\n");
    if(actualLength != finalLength - 1){            // finalLength computes the '\0' while actualLength doesn't
        printErr("Error concating the strings");
        return;
    }
}

static void cpufeaturesHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 3){
        printErr("Too many parameters for command 'cpufeatures'");
        return;
    }
    CommonFeatures commonFeatures;
    ExtendedFeatures extendedFeatures;
    if(!getCpuFeatures(&commonFeatures, &extendedFeatures)){
        printErr("cpuid instruction not supported");
        return;
    }
    printFeatures(commonFeatures, extendedFeatures);
    return;
}

static void inforegHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 3){
        printErr("Too many parameters for command 'inforeg'");
        return;
    }
    Registries regs;
    getRegistries(&regs);
    if (regs.RIP == 0){
        printErr("The registries have not been captured yet. press the [~] key to capture before executing 'inforeg'.");
    } else {
        printRegistries(regs);
    }
    return;
}

static void printmemHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 3){
        printErr("Missing parameter for command 'printmem'");
        return;
    }
    if(paramAmount > 3){
        printErr("Too many parameters for command 'printmem'");
        return;
    }
    uint64_t memPos = strToNumPos(params[0]);
    if((int64_t)memPos == -1)
        return;
    char memContent[PRINTMEM_BYTES];
    getMemContent(memPos, (uint8_t*)memContent, PRINTMEM_BYTES);
    printMemContent(memPos, (uint8_t*)memContent, PRINTMEM_BYTES);
}

static void memHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 3){
        printErr("Too many parameters for command 'mem'");
        return;
    }
    MemoryInfo meminfo;
    getMemInfo(&meminfo);
    printMemInfo(meminfo, params[0]);
}

// for testing purposes
static void testallocHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 3){
        printErr("Missing parameter for command 'testalloc'");
        return;
    }
    if(paramAmount > 3){
        printErr("Too many parameters for command 'testalloc'");
        return;
    }
    int num = strToNum(params[0]);
    void* mem = malloc(sizeof(int));
    if(mem == NULL)
        return;
    *((int*)mem) = num;

    int num2 = *((int*)mem);
    printInt(num2, 10, 10);
    printf("\nMemory allocated at: ");
    printInt((long)mem, 16, 16);
    printf("\n");
    /* calloc test */
    /*
        uint8_t* mem = (uint8_t*)calloc(10, 10);
        for(int i=0 ;  ; i++){
            if(i == 100){
                printf("All of memory was initialized to zero\n");
                break;
            }
            if(*mem != 0){
                printf("Memory was not fully initialized to zero\n");
                break;
            }
        }
    */
    free(mem);
    return;
}

void psHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 2){
        printErr("Too many parameters for command 'ps'");
        return;
    }
    printAllProcesses();
    return;
}

int loop(int argc, char* argv[]) {
    int pid = getpid();
    size_t seconds = strToNum(argv[0]);
    while(1){
        sleep(pid, seconds);
        printf("Hi, process ");
        printInt(pid,2,10);
        printf(" here\n");
    }
    return 0;
}

void loopHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 2){
        printErr("Missing parameter for command 'loop'");
        return;
    }
    if(paramAmount > 3){
        printErr("Too many parameters for command 'loop'");
        return;
    }
    char* time[paramAmount];
    time[0] = params[0];
    if(createFullProcess(loop, LOW, 1, time, "loop") == -1){
        printErr("Cannot create a new process; process limit reached");
        return;
    }
    return;
}

/****           PHYLO PROGRAM           ****/

#define MAX_PHYLOS      5
#define MAX_NAME_LEN    100
#define LEFT            (phid + (MAX_PHYLOS-1)) % MAX_PHYLOS
#define RIGHT           (phid + 1) % MAX_PHYLOS

typedef enum {THINKING, HUNGRY, EATING} PhyloState;

PhyloState state[MAX_PHYLOS] = {0};
char* tableSem = "table";
char* idSem = "phylo_id";
char* chopstickSem[] = {"chopstick1", "chopstick2", "chopstick3", "chopstick4", "chopstick5"};
int phyloBaseId = 0;

static int left(int id){
    return (id == 0) ? MAX_PHYLOS-1 : id-1;
}

static int right(int id){
    return (id == MAX_PHYLOS-1) ? 0 : id+1;
}

void printPhylos(){
    for(int i=0 ; i < MAX_PHYLOS ; i++){
        if(state[i] == EATING)
            printf("E");
        else
            printf("-");
        printf(" ");
    }
    printf("\n");
}

void test(int phid)
{
    if (state[phid] == HUNGRY && state[left(phid)] != EATING && state[right(phid)] != EATING) {

        // state that eating
        state[phid] = EATING;

        // sem_post(&S[phnum]) has no effect during takefork
        // used to wake up hungry philosophers during putfork
        sem_post(chopstickSem[phid]);
    }
}

void take_chopsticks(int phid)
{
    sem_wait(tableSem);

    // state that hungry
    state[phid] = HUNGRY;
 
    // eat if neighbours are not eating
    test(phid);
 
    sem_post(tableSem);
    // if unable to eat wait to be signalled
    sem_wait(chopstickSem[phid]);
}

void put_chopsticks(int phid)
{
    sem_wait(tableSem);
 
    // state that thinking
    state[phid] = THINKING;
 
    test(left(phid));
    test(right(phid));
 
    sem_post(tableSem);
}

static int phylo(int argc, char* argv[]) //void* num
{
    sem_wait(idSem);
    int phyloId = phyloBaseId % 5;
    phyloBaseId++;
    sem_post(idSem);
    while(1){
        sleep(getpid(), 1);
        take_chopsticks(phyloId);
        printPhylos();
        sleep(getpid(), 1);
        put_chopsticks(phyloId);
    }
    return 0;
}

void phyloHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 1){
        printErr("Missing parameter for command 'phylo'");
        return;
    }
    sem_open(tableSem, 1);
    sem_open(idSem, 1);
    for(int i=0 ; i < MAX_PHYLOS ; i++){
        sem_open(chopstickSem[i], 0);
    }
    for(int i=0 ; i < MAX_PHYLOS ; i++){
        // char id[10];
        // numToStr(i, id, 10);
        // char argv[][MAX_NAME_LEN] = {id};
        createProcess(phylo, "phylo");
        // printf("Creo al filosofo #");
        // printInt(i, 10, 10);
        // printf("\n");
    }
}

void killHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 2){
        printErr("Missing parameter for command 'kill'");
        return;
    }
    if(paramAmount > 3){
        printErr("Too many parameters for command 'kill'");
        return;
    }
    int pid = strToNum(params[0]);
    if(killProcess(pid) == -1){
        printErr("No process with pid sent");
        return;
    }
}

void niceHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 2){
        printErr("Missing parameters for command 'nice'");
        return;
    }
    if(paramAmount == 3){
        printErr("Missing process new priority");
        return;
    }
    if(paramAmount > 4){
        printErr("Too many parameters for command 'nice'");
        return;
    }
    int pid = strToNum(params[0]);
    int priority = strToNum(params[1]);
    if(nice(pid, priority) == -1){
        printErr("Cannot assign new priority to process with pid sent");
        return;
    }
}

void blockHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 2){
        printErr("Missing parameters for command 'block'");
        return;
    }
    if(paramAmount > 3){
        printErr("Too many parameters for command 'block'");
        return;
    }
    int pid = strToNum(params[0]);
    if(blockProcess(pid) == -1){
        printErr("Cannot change sent process state");
        return;
    }
}

void unblockHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 2){
        printErr("Missing parameters for command 'unblock'");
        return;
    }
    if(paramAmount > 3) {
        printErr("Too many parameters for command 'unblock'");
        return;
    }
    int pid = strToNum(params[0]);
    if(unblockProcess(pid) == -1){
        printErr("Cannot change sent process state");
        return;
    }
}

void skipHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 2){
        printErr("Too many parameters for command 'skip'");
        return;
    }
    skipExecution();
}

void semHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 2){
        printErr("Too many parameters for command 'sem'");
        return;
    }
    printAllSemaphores();
    return;
}

void catHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 2){
        printErr("Too many parameters for command 'cat'");
        return;
    }
    int fd_in = strToNum(params[0]);
    int fd_out = strToNum(params[1]);
    char input[120];
	
    scanfd(fd_in,input,120);     
    printfd(fd_out,input);
    return;
}

void wcHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 2){
        printErr("Too many parameters for command 'wc'");
        return;
    }
    int fd_in = strToNum(params[0]);
    int fd_out = strToNum(params[1]);
    char input[120];

    scanfd(fd_in,input,120);    
    int lines = 0;
	for(int i=0; i<120 ;i++) {
        if(input[i] == '\n') {
            lines++;
        }
    }
    char lineString[5] = "";
    numToStr(lines,lineString,5);
    printfd(fd_out,lineString);
    printfd(fd_out,"\n");
    return;
}

void filterHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 2){
        printErr("Too many parameters for command 'filter'");
        return;
    }
    int fd_in = strToNum(params[0]);
    int fd_out = strToNum(params[1]);
    char input[120];
    
    scanfd(fd_in,input,120);
	int j=0;
	for(int i=0; i<120 ;i++) {
        if(input[i] != 'a' && input[i] != 'e' && input[i] != 'i' && input[i] != 'o' && input[i] != 'u') {
            input[j] = input[i];
            j++;
        }
    }
    printfd(fd_out,input);
    return;
}

void pipeHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 2){
        printErr("Too many parameters for command 'pipe'");
        return;
    }
    //int fd_out = strToNum(params[1]);
    printPipes();
    return;
}

static void sleepHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount < 4){
        printErr("Missing parameters for command 'sleep'");
        return;
    }
    if(paramAmount > 4){
        printErr("Too many parameters for command 'sleep'");
        return;
    }
    int pid = strToNum(params[0]);
    size_t seconds = strToNum(params[1]);
    if(sleep(pid, seconds) == -1) {
        printErr("Cannot sleep given process pid");
        return;
    }
}

static void clearHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 2){
        printErr("Too many parameters for command 'clear'");
        return;
    }
    clear();
}

int testProcessA(int argc, char* argv[]){
    char test[20] = "Writing ProcessA\n";
    int id = strToNum(argv[0]);
    if(writePipe(id,test,20) == -1) {
        printf("Error writing in pipe\n");
        return 0;
    }
    return 0;
}

int testProcessB(int argc, char* argv[]){
    char test[20];
    int id = strToNum(argv[0]);
    if(readPipe(id,test,20) == -1) {
        printf("Error reading in pipe\n");
        return 0;
    }
    return 0;
}

int testProcessC(int argc, char* argv[]){
    int id = 6;
    void *mem = getSharedMem(id);
    int num = 24;
    *((int*) mem) = num;
    return 0;
}

int testProcessD(int argc, char* argv[]){
    int id = 6;
    void *mem = getSharedMem(id);
    printf("El numero es ");
    printInt(*((int*) mem),10,10);
    return 0;
}

void testProcessHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount > 2){
        printErr("Too many parameters for command 'test'");
        return;
    }
    /*int pipe = createPipe();
    char *id;
    numToStr(pipe,id,10);
    char* argv[] = {id};*/
    if(createFullProcess(testProcessC, LOW, 0, NULL, "sharedMemC") == -1){
        printErr("Cannot create a new process; process limit reached");
        return;
    }
    if(createFullProcess(testProcessD, LOW, 0, NULL, "sharedMemD") == -1){
        printErr("Cannot create a new process; process limit reached");
        return;
    }
}

/****               TEST_MM             ****/
#define MAX_BLOCKS 10
#define MAX_MEMORY 0x19266666 //80% of memory managed by the MM

typedef struct MM_rq{
  void *address;
  uint32_t size;
} mm_rq;

int test_mm(int argc, char* argv[]){
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint64_t total;

    rq = 0;
    total = 0;

    printf("\nREQUESTING BLOCKS...\n");
    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
        mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
        printf("Request block #");
        printInt(rq, 10, 10);
        printf(" of 0x");
        printInt(mm_rqs[rq].size, 10, 16);
        printf("b\n");
        mm_rqs[rq].address = malloc(mm_rqs[rq].size); // TODO: Port this call as required
        if(mm_rqs[rq].address == NULL){
            printErr("No more memory left\n");
            return -1;
        }
        printf("on addr: ");
        printInt((size_t)mm_rqs[rq].address, 10, 16);
        printf("\n");
        //TODO: check if NULL
        total += mm_rqs[rq].size;
        rq++;
    }

    printf("\nSETTING BLOCKS...\n");
    // Set
    for (uint32_t i = 0; i < rq; i++){
        printf("Setting block #");
        printInt(i, 10, 10);
        printf("\n");
        if (mm_rqs[i].address != NULL){
            for(int j=0 ; j < mm_rqs[i].size ; j++)
                *(char*)(mm_rqs[i].address+j) = i;
        }
        printf("Block #");
        printInt(i, 11, 10);
        printf(" was successfully set\n");
    }

    printf("\nCHECKING BLOCKS...\n");
    // Check
    for (uint32_t i = 0; i < rq; i++){
        if (mm_rqs[i].address != NULL){
            if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)){
                printf("ERROR checking block #");
                printInt(i, 10, 10);
                printf("\n");
            }
            else{
                printf("Block #");
                printInt(i, 10, 10);
                printf(" successfully checked\n");
            }
        }
    }

    printf("\nFREEING BLOCKS...\n");
    // Free
    for (uint32_t i = 0; i < rq; i++){
        if (mm_rqs[i].address != NULL){
            free(mm_rqs[i].address);
            printf("Freeing block #");
            printInt(i, 10, 10);
            printf("\n");
        }
    }
    printf("\nFINISH mm test\n");
    return 0; 
}

/****           TEST_PROCESSES          ****/
#define MAX_PROCESSES 16

enum State {ERROR, RUNNING, BLOCKED, KILLED};

typedef struct P_rq{
  uint32_t pid;
  enum State state;
}p_rq;

int endless_loop(int argc, char* argv[]){
    while(1);
    return 0;
}

int test_processes(int agrc, char* argv[]){
    p_rq p_rqs[MAX_PROCESSES];
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;

    printf("\nCREATING PROCESSES...\n");
    // Create MAX_PROCESSES processes
    for(rq = 0; rq < MAX_PROCESSES; rq++){
        p_rqs[rq].pid = createProcess(endless_loop, "endless_loop");
        if (p_rqs[rq].pid == -1){                           // TODO: Port this as required
            printf("Error creating process #");
            printInt(rq, 10, 10);
            printf("\n");               // TODO: Port this as required
            return -1;
        } else {
            p_rqs[rq].state = RUNNING;
            printf("Created process #");
            printInt(rq, 10, 10);
            printf("\n");
            alive++;
        }
    }

    printf("\nRANDOM ACTIONS ON PROCESSES...\n");
    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0){
        for(rq = 0; rq < MAX_PROCESSES; rq++){
            action = GetUniform(2) % 2; 
            switch(action){
                case 0:
                    if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED){
                        if (killProcess(p_rqs[rq].pid) == -1){
                            printf("Error killing process #");
                            printInt(rq, 10, 10);
                            printf("\n");
                            return -1;
                        }
                        p_rqs[rq].state = KILLED;
                        printf("Killed process #");
                        printInt(rq, 10, 10);
                        printf("\n");
                        alive--;
                    }
                    break;
                case 1:
                    if (p_rqs[rq].state == RUNNING){
                        if(blockProcess(p_rqs[rq].pid) == -1){
                            printf("Error blocking process #");
                            printInt(rq, 10, 10);
                            printf("\n");
                            return -1;
                        }
                        p_rqs[rq].state = BLOCKED;
                        printf("Blocked process #");
                        printInt(rq, 10, 10);
                        printf("\n");
                    }       
                break;
            }
        }
    
        // Randomly unblocks processes
        for(rq = 0; rq < MAX_PROCESSES; rq++){
            if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2){
                if(unblockProcess(p_rqs[rq].pid) == -1){            // TODO: Port this as required
                    printf("Error unblocking process #");
                    printInt(rq, 10, 10);
                    printf("\n");         // TODO: Port this as required
                    return -1;
                }
                p_rqs[rq].state = RUNNING; 
                printf("Unblocked process #");
                printInt(rq, 10, 10);
                printf("\n");
            }
        }
    }
    printf("\nFINISH processes test\n");
    return 0;
}

/****           TEST_PRIORITY           ****/
#define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice

void bussy_wait(uint64_t n){     //u64int_t n
    uint64_t i;
    for (i = 0; i < n; i++);
}

int endless_loop_2(int argc, char* argv[]){
    uint64_t pid = getpid();

    while(1){
        printInt(pid, 10, 10);
        printf("\n");
        bussy_wait(MINOR_WAIT);
    }
    return 0;
}

#define TOTAL_PROCESSES 3

void test_priority(){
    uint64_t pids[TOTAL_PROCESSES];
    uint64_t i;

    for(i = 0; i < TOTAL_PROCESSES; i++){
        pids[i] = createProcess(endless_loop_2, "endless_loop_2");      // By default, on lowest priority (0)
        if(pids[i] == -1){
            printf("Error creating process #");
            printInt(i, 10, 10);
            printf("\n");
            return;
        }
        printf("Created process #");
        printInt(i, 10, 10);
        printf("\n");
    }

    bussy_wait(WAIT);
    printf("\nCHANGING PRIORITIES...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++){
        switch (i % 3){
            case LOW:
                if(nice(pids[i], LOW) == -1) {          //lowest priority
                    printf("Error changing process #");
                    printInt(i, 10, 10);
                    printf(" priority\n");
                    return;
                }
                break;
            case MEDIUM:
                if(nice(pids[i], MEDIUM) == -1){        //medium priority
                    printf("Error changing process #");
                    printInt(i, 10, 10);
                    printf(" priority\n");
                    return;
                }
                break;
            case HIGH:
                if(nice(pids[i], HIGH) == -1){ //highest priority
                    printf("Error changing process #");
                    printInt(i, 10, 10);
                    printf(" priority\n");
                    return;
                }
                break;
        }
        printf("Changed process #");
        printInt(i, 10, 10);
        printf(" priority\n");
    }

    bussy_wait(WAIT);
    printf("\nBLOCKING...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++){
        if(blockProcess(pids[i]) == -1){
            printErr("Error blocking process #");
            printInt(i, 10, 10);
            printf("\n");
            return;
        }
        printf("Blocked process #");
        printInt(i, 10, 10);
        printf("\n");
    }

    printf("\nCHANGING PRIORITIES WHILE BLOCKED...\n");
    for(i = 0; i < TOTAL_PROCESSES; i++){
        switch (i % 3){
            case LOW:
                if(nice(pids[i], LOW) == -1) {          //lowest priority
                    printf("Error changing process #");
                    printInt(i, 10, 10);
                    printf(" priority\n");
                    return;
                }
                break;
            case MEDIUM:
                if(nice(pids[i], MEDIUM) == -1){        //medium priority
                    printf("Error changing process #");
                    printInt(i, 10, 10);
                    printf(" priority\n");
                    return;
                }
                break;
            case HIGH:
                if(nice(pids[i], HIGH) == -1){ //highest priority
                    printf("Error changing process #");
                    printInt(i, 10, 10);
                    printf(" priority\n");
                    return;
                }
                break;
        }
        printf("Changed process #");
        printInt(i, 10, 10);
        printf(" priority\n");
    }

    printf("\nUNBLOCKING...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++){
        if(unblockProcess(pids[i]) == -1){
            printErr("Error unblocking process #");
            printInt(i, 10, 10);
            printf("\n");
            return;
        }
        printf("Unblocked process #");
        printInt(i, 10, 10);
        printf("\n");
    }

    bussy_wait(WAIT);
    printf("\nKILLING...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++){
        if(killProcess(pids[i]) == -1){
            printErr("Error killing process #");
            printInt(i, 10, 10);
            printf("\n");
            return;
        }
        printf("Killed process #");
        printInt(i, 10, 10);
        printf("\n");
    }
    printf("\nFINISH priority test\n");
}

/****           TEST_SYNC & TEST_NO_SYNC           ****/
#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

int64_t global;  //shared memory

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  // yield();
  *p = aux;
}

int inc(int argc, char* argv[]){
    uint64_t sem = strToNum(argv[0]);
    int64_t value = strToNum(argv[1]);
    uint64_t N = strToNum(argv[2]);
    uint64_t i;

    if (sem && sem_open(SEM_ID, 1) == -1){
        printf("ERROR OPENING SEM\n");
        return -1;
    }
  
    for (i = 0; i < N; i++){
        if (sem) sem_wait(SEM_ID);
        slowInc(&global, value);
        if (sem) sem_post(SEM_ID);
    }

    if (sem){
        sem_destroy(SEM_ID);
    }
  
    printf("Final value: ");
    printInt(global, 10, 10);
    printf("\n");
    return 0;
}

void test_sync(){
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITH SEM)\n");

  char* argv1[] = {"1", "1", "1000000"}; //1000000
  char* argv2[] = {"1", "-1", "1000000"}; //1000000

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    createFullProcess(inc, HIGH, 3, argv1, "inc");
    createFullProcess(inc, HIGH, 3, argv2, "inc");
  }
}

void test_no_sync(){
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITHOUT SEM)\n");

  char* argv1[] = {"0", "1", "1000000"};
  char* argv2[] = {"0", "-1", "1000000"};

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    createFullProcess(inc, HIGH, 3, argv1, "inc");
    createFullProcess(inc, HIGH, 3, argv2, "inc");
  }
}

static size_t testProgramAmount = 5;

static testProgramStruct testPrograms[] = {
    {"mm", &test_mm},
    {"processes", &test_processes},
    {"priority", &test_priority},
    {"sync", &test_sync},
    {"nosync", &test_no_sync}
};

static void testHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 2){
        printErr("Missing parameters for command 'sleep'");
        return;
    }
    if(paramAmount > 3){
        printErr("Too many parameters for command 'sleep'");
        return;
    }
    for(int i=0 ; i < testProgramAmount ; i++){
        if(strcmp(params[0], testPrograms[i].name) == 0){
            createProcessWithPriority(testPrograms[i].entryPoint, HIGH, testPrograms[i].name);
            return;
        }
    }
    printErr("Not test program found with name sent\n");
}

static void printCommandTypeMessage(commandStruct command){
    printf(command.name);
    printf(" is a ");
    switch(command.type){
        case BUILT_IN:
            printf("shell built-in");
            break;
        case PROCESS:
            printf("process");
            break;
        default:
            break;
    }
    printf("\n");
}

static void typeHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount);
static void helpHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount);

static const size_t commandAmount = 25;
static commandStruct commands[] = {
    {"help", &helpHandler, "'help': Get information on how to use commands\nUse: 'help [command]'\n'command': Command to get use information about\n", BUILT_IN},
    {"echo", &echoHandler, "'echo': Print a message on the console\nUse: 'echo [message]'\n'message': Message to print in console\n", BUILT_IN},
    {"cpufeatures", &cpufeaturesHandler, "'cpufeatures': Print cpu support for key features like mmx, sse, avx, etc\nUse: 'cpufeatures'\n", BUILT_IN},
    // {"exception", &testHandler, "'exception': Throws the provided exception\nUse: 'test [exception]'\n'exception': Type of exception to be thrown\nAvailable exceptions:\ndiv-by-zero\ninvalid-opcode\n", BUILT_IN},
    {"inforeg", &inforegHandler, "'inforeg': Print the states of the registries\nUse: 'inforeg'\n", BUILT_IN},
    {"printmem", &printmemHandler, "'printmem': Print the first 32 bytes following a place in memory\nUse: 'printmem [pointer]'\n'pointer': Memory address of first byte to print\n", BUILT_IN},
    {"mem", &memHandler, "'mem': Displays memory info\nUse: 'mem' [units]\n'units': Determines which unit the info will be displayed on\nOptions:\n-b: Bytes (Default)\n-k: Kilobytes\n", BUILT_IN},
    {"testalloc", &testallocHandler, "'testalloc': Tests the functionality of memory allocation\nUse: 'testalloc' [test num]\n'test num': A test integer number that will be saved in memory and then read\n", BUILT_IN},
    {"ps", &psHandler, "'ps': Displays running process information\n", BUILT_IN},
    {"loop", &loopHandler, "'loop': Displays process Id and a greeting every few seconds", PROCESS},
    {"kill", &killHandler, "'kill': Kills process with pid sent\nUse: 'kill' [pid]\n'pid': Id of process\n", BUILT_IN},
    {"nice", &niceHandler, "'nice': Change priority of given process\nUse: 'nice' [pid] [priority]\n'pid': Id of process\n'priority': New priority to assign to process\n", BUILT_IN},
    {"block", &blockHandler, "'block': Blocks a process\nUse: 'block [pid]'\n'pid': Id of process\n", BUILT_IN},
    {"unblock", &unblockHandler, "'block': Unblocks a process\nUse: 'unblock [pid]'\n'pid': Id of process\n", BUILT_IN},
    {"skip", &skipHandler, "'skip': Skips execution of current process. Do not confound with 'kill'\nUse: 'skip'\n", BUILT_IN},
    {"sem", &semHandler, "'sem': Displays current semaphores information\nUse: sem [?] ...", BUILT_IN},
    {"cat", &catHandler, "'cat': Waits for user to input something and replies it back\nUse: 'cat'\n", PROCESS},
    {"wc", &wcHandler, "'wc': Prints amount of lines of input recieved\nUse: 'wc'\n", PROCESS},
    {"filter", &filterHandler, "'filter': Waits for user to input something and replies ti back filtering vowels\nUse: 'filter'", PROCESS},
    {"pipe", &pipeHandler, "'pipe': Prints system pipes\nUse: 'pipe'", BUILT_IN},
    {"phylo", &phyloHandler, "'phylo': Philosophers program. Recieves philosophers count\nUse: 'phylo [count]'\n'count': Number of philosophers\n", PROCESS},
    {"clear", &clearHandler, "'clear': Clears the current console\nUse: 'clear'\n", BUILT_IN},
    {"sleep", &sleepHandler, "'sleep': Causes the given process to sleep for the seconds specified\nUse: 'sleep [pid] [seconds]'\n'pid': Process id\n'seconds': Number of seconds for the system to sleep\n", BUILT_IN},    
    {"testprocess", &testProcessHandler, "'testprocess': Creates new process\nUse: 'testprocess'\n", PROCESS},
    {"type", &typeHandler, "'type': Prints a command type\nUse: 'type [command]'\n'command': Command name\n", BUILT_IN},
    {"test", &testHandler, "'test': Runs given test program\nUse: 'test [program]'\n'program': Must be one of the following"
    "- 'mm': Memory manager test\n- 'processes': Process creation, blocking and killing test\n- 'priority': Process priority test\n", PROCESS}
};

static void typeHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if(paramAmount == 2){
        printErr("Missing parameter for command 'type'");
        return;
    }
    if(paramAmount > 3){
        printErr("Too many parameters for command 'type'");
        return;
    }
    for(int i=0 ; i < commandAmount ; i++){
        if(strcmp(commands[i].name, params[0]) == 0){
            printCommandTypeMessage(commands[i]);
            return;
        }
    }
    printErr("No command with sent name was found");
    return;
}

static void helpHandler(char params[][MAX_PARAMETER_LENGTH], size_t paramAmount){
    if (paramAmount > 3){
        printErr("Too many parameters for command 'help'\nUse: help [command]");
    }
    if (paramAmount == 2){
        printf("Available Commands:\n");
        printf("help [command]\n");
        for(int i=1; i<commandAmount; i++) {
            printf(commands[i].name);
            printf("\n");
        }
    } else {
        for(int i=0 ; i < commandAmount ; i++){
            if(strcmp(commands[i].name, params[0]) == 0){
                printf(commands[i].help_message);
            }
        }
    }
}

static int isEnd(int c){
    if (c == '\n' || c == 0) return 1;
    return 0;
}

static int getCommandAndParams(char* command1, char params1[][MAX_PARAMETER_LENGTH], int *params1Idx, 
                               char* command2, char params2[][MAX_PARAMETER_LENGTH], int *params2Idx,
                               char* input) {
    int param1Idx=0; 
    int param2Idx=0;    
    int j=0;
    int fd=0;
    int state = COMMAND1;
    for(int inputIdx=0; !isEnd(input[inputIdx]); inputIdx++) {
        switch (state) {
            case COMMAND1:
                if(input[inputIdx] == ' ') {
                    if(j == 0) {
                        //ignore space
                    } else {
                        command1[j] = 0;
                        j=0;
                        state = PARAM1;
                    } 
                } else if(input[inputIdx] == '+' && j != 0) {
                    command1[j] = 0;
                    j=0;
                    state = COMMAND2;
                    fd = PIPED;
                } else {
                    command1[j++] = input[inputIdx];
                    if(isEnd(input[inputIdx + 1])) {
                        command1[j] = 0;
                    }
                }
                break;
            
            case PARAM1:
                if(input[inputIdx] == ' ') {
                    if(j == 0) {
                        //ignore space
                    } else {
                        params1[param1Idx++][j] = 0;
                        j=0;
                    }
                } else if (input[inputIdx] == '+') {
                    params1[param1Idx][j] = 0;
                    j=0;
                    state = COMMAND2;
                    fd = PIPED;
                } else {
                    params1[param1Idx][j++] = input[inputIdx];
                    if(isEnd(input[inputIdx + 1])) {
                        params1[param1Idx++][j] = 0;
                    }
                }
                break;

            case COMMAND2:
                if(input[inputIdx] == ' ') {
                    if(j == 0) {
                        //ignore space
                    } else {
                        command2[j] = 0;
                        j=0;
                        state = PARAM2;
                    } 
                } else {
                    command2[j++] = input[inputIdx];
                    if(isEnd(input[inputIdx + 1])) {
                        command2[j] = 0;
                    }
                }
                break;
        
            case PARAM2:
                if(input[inputIdx] == ' ') {
                    if(j == 0) {
                        //ignore space
                    } else {
                        params2[param2Idx++][j] = 0;
                        j=0;
                    }
                } else {
                    params2[param2Idx][j++] = input[inputIdx];
                    if(isEnd(input[inputIdx + 1])) {
                        params2[param2Idx++][j] = 0;
                    }
                }
                break;
        }
        *params1Idx = param1Idx;
        *params2Idx = param2Idx;
    }
    return fd;
}

void executeCommand(char commandName[MAX_COMMAND_LENGTH+1], char params[MAX_PARAMETER_AMOUNT][MAX_PARAMETER_LENGTH], int paramAmount) {
    /*
    int isBackground = 0;
    char* name = commandName;
    if(strcmp("&", (char*)name[0]) == 0){
        name++;
        isBackground = 1;
    }
    */
    for(int i=0 ; i < commandAmount ; i++){
        if(strcmp(commands[i].name, commandName) == 0){
            setIdle(0);
            ((void(*)(char[][MAX_PARAMETER_LENGTH], size_t))commands[i].handler)(params, paramAmount);
            printf("> ");
            setIdle(1);
            return;
        }
    }
    printErr("Unknown command. Use 'help' for a list of commands");
    printf("> ");
    return;
}

void commandHandler(char* string){
    char commandName1[MAX_COMMAND_LENGTH+1] = "";
    char commandName2[MAX_COMMAND_LENGTH+1] = "";
    char params1[MAX_PARAMETER_AMOUNT][MAX_PARAMETER_LENGTH];
    char params2[MAX_PARAMETER_AMOUNT][MAX_PARAMETER_LENGTH];
    int paramsAmount1;
    int paramsAmount2;
    char fd_pipe[5] = "";
    char fd_standard[5] = "0";
    if(getCommandAndParams(commandName1, params1, &paramsAmount1, commandName2, params2, &paramsAmount2, string) == PIPED) {
        int id = createPipe();
        numToStr(id, fd_pipe, 5);
        for(int i=0; i < 5; i++) {
            params1[paramsAmount1][i] = fd_standard[i];
            params2[paramsAmount2][i] = fd_pipe[i];
        }
        paramsAmount1++;
        paramsAmount2++;
        for(int i=0; i < 5; i++) {
            params1[paramsAmount1][i] = fd_pipe[i];
            params2[paramsAmount2][i] = fd_standard[i];
        }
        paramsAmount1++;
        paramsAmount2++;
        executeCommand(commandName1, params1, paramsAmount1);
        executeCommand(commandName2, params2, paramsAmount2);
    } else {
        for(int j=0; j<2 ; j++) {
            for(int i=0; i < 5; i++) {
                params1[paramsAmount1][i] = fd_standard[i];
            }
            paramsAmount1++;
        }
        executeCommand(commandName1, params1, paramsAmount1);
    }
    return;
}