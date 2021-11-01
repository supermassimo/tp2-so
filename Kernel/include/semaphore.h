#include <scheduler.h>
#include <memManager.h>

typedef struct {
    int pid;
    WProcess *next;
} WProcess;

typedef struct {
    char *id;
    int value;
    WProcess *wp;
    sem_t *next;
} sem_t;

int sem_init(char *sem_id, int value);
int sem_destroy(char *sem_id);
int sem_wait(char *sem_id);
int sem_post(char *sem_id);
int sem_set_value (char *sem_id, int value);
