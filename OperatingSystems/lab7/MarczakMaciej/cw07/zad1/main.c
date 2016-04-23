#include "headers.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int *childs;
int N;
int mem_id, sem_id;

void finish_tasks(int sig) {
    int i;
    for (i = 0; i < N; i++) {
        kill(childs[i], SIGUSR1);
    }

    semctl(sem_id, 0, IPC_RMID, NULL);
    shmctl(mem_id, IPC_RMID, NULL);

    exit(1);
}

int main() {

    // TODO: read from command lines
    int m = 3, n = 2;
    N = m + n;

    childs = malloc( (m + n) * sizeof(int) );

    key_t key;
    unsigned short init_vals[MAX_TASKS + 1];

    key = ftok(MEM_PATH, MEM_ID);
    mem_id = shmget(key, (MAX_TASKS + 1) * sizeof(int), IPC_CREAT | IPC_EXCL | 0700);
    int *array = (int *) shmat(mem_id, (void *) 0, 0);

    int i;
    for (i = 0; i <= MAX_TASKS; i++) {
        array[i] = 0;
        init_vals[i] = 1;
    }

    shmdt(array);

    key = ftok(SEM_PATH, SEM_ID);
    sem_id = semget(key, MAX_TASKS + 1, IPC_CREAT | 0700);

    pid_t pid;
    union semun init;
    init.array = init_vals;

    semctl(sem_id, 0, SETALL, init);

    int j;
    for (j = 0; j < m; j++) {
        if ( (pid = fork()) == 0 ) {
            execl("./producent", "producent", (char *) 0);
            exit(1);
        } else {
            childs[j] = (int) pid;
        }
    }

    for (j = 0; j < n; j++) {
        if ( (pid = fork()) == 0 ) {
            execl("./consumer", "consumer", (char *) 0);
            exit(1);
        } else {
            childs[m + j] = (int) pid;
        }
    }

    signal(SIGINT, finish_tasks);

    while(1) {
        // endless loop
    }

}
