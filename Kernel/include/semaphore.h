#include <scheduler.h>
#include <memManager.h>

int sem_init(int value);
int sem_destroy(int sem_id);
void sem_wait(int sem_id);
void sem_post(int sem_id);
void sem_set_value (int sem_id, int value);
