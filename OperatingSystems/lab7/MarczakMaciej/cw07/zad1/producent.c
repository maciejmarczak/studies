#include "headers.h"

struct sembuf take, release, take_m, release_m;

void init_sem() {
    take.sem_op = take_m.sem_op = -1;
    release.sem_op = release_m.sem_op = 1;

    take.sem_flg = release.sem_flg = IPC_NOWAIT;

    take_m.sem_num = release_m.sem_num = MAX_TASKS;
}

int main() {
    srand(time(NULL));

    int mem_id = get_mem_id();
    int sem_id = get_semaphores();
    int *array = shmat(mem_id, NULL, 0);
    int pid = (int) getpid();

    signal(SIGUSR1, exit);

    init_sem();

    int i = 0;
    while(1) {

        take.sem_num = release.sem_num = i;

        if ( semop(sem_id, &take, 1) == 0 ) {
            if ( array[i] == 0 ) {
                array[i] = rand() % MAXRAND;

                semop(sem_id, &take_m, 1);
                array[MAX_TASKS] += 1;
                printf( "(PID: %d\tTIMESTAMP: %u)\tDodalem liczbe "
                        "%d na pozycje %d. Liczba zadan oczekujacych: %d.\n",
                        pid, (unsigned) time(NULL), array[i], i, array[MAX_TASKS]);

                semop(sem_id, &release_m, 1);
            }

            semop(sem_id, &release, 1);
        }

        i += 1;
        i %= MAX_TASKS;

    }

    return 0;
}
