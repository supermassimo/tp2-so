#include <scheduler.h>
#include <memManager.h>

// NICO: Massi asignale los valores cuando crees el semaforo, asignandolos acá da error al compilar
typedef struct {
    int value;
    int waitingProcesses[MAX_PROCESSES];
    int waitingProcessCount;
    /*
    int waitingProcesses[MAX_PROCESSES] = {0};
    int waitingProcessesCount = 0;
    */
} sem_t;

