#include <scheduler.h>
#include <memManager.h>
#include <lib.h>
#include <console.h>

#define MAX_ID_LENGTH 50

typedef struct WProcess{
    int pid;
    struct WProcess *next;
} WProcess;

typedef struct sem_t{
    char *id;
    int value;
    WProcess *wp;
    struct sem_t *next;
} sem_t;

int sem_open(const char *sem_id, int value);
int sem_destroy(const char *sem_id);
int sem_wait(char *sem_id);
int sem_post(const char *sem_id);
int sem_set_value (const char *sem_id, int value);
int sem_get_value (const char *sem_id);

void printAllSemaphores();
