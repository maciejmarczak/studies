#include "headers.h"

int get_mem_id() {
    key_t key = ftok(MEM_PATH, MEM_ID);

    return shmget(key, (MAX_TASKS + 1) * sizeof(int), 0700);
}

int get_semaphores() {
    key_t key = ftok(SEM_PATH, SEM_ID);

    return semget(key, MAX_TASKS + 1, 0700);
}
