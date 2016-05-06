#include "headers.h"

sem_t* sem;

void exit_fun() {
    shm_unlink(MEM_NAME);
    sem_close(sem);
    sem_unlink(SEM_NAME);
}

int main() {
    srand(time(NULL));
    int m = 5, n = 10;

    atexit(exit_fun);
    signal(SIGINT, exit);

    int mem_fd = shm_open(MEM_NAME, O_CREAT | O_EXCL | O_RDWR, 0777);
    ftruncate(mem_fd, ARRAY_SIZE * sizeof(int));
    int *array = (int *) mmap(0, ARRAY_SIZE * sizeof(int), PROT_WRITE, MAP_SHARED, mem_fd, 0);

    sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0777, 1);

    int i;

    for (i = 0; i < m; i++) {
        if ( fork() == 0 ) {
            execl("./writer", "writer", (char *) 0);
            exit(1);
        }

        sleep(1);
    }

    for (i = 0; i < n; i++) {
        if ( fork() == 0 ) {
            execl("./reader", "reader", (char *) 0);
            exit(1);
        }

        sleep(1);
    }

    while(1) {

    }
}
