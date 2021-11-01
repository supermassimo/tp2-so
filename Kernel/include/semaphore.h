#include <scheduler.h>
#include <memManager.h>
#include <lib.h>

#define MAX_ID_LENGTH 50

typedef struct WProcess{
    int pid;
    struct WProcess *next;
} WProcess;

typedef struct sem_t{
    char id[MAX_ID_LENGTH];
    int value;
    WProcess *wp;
    struct sem_t *next;
} sem_t;

int sem_init(char *sem_id, int value);
int sem_destroy(char *sem_id);
int sem_wait(char *sem_id);
int sem_post(char *sem_id);
int sem_set_value (char *sem_id, int value);
