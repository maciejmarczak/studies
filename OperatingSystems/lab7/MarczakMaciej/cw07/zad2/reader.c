#include "headers.h"

int main(int argc, char **argv) {
    srand(time(NULL));

    int x = 0;
    if (argc == 2) x = atoi(argv[1]);
    if (x == 0) x = rand() % MAX_RAND;

    int mem_fd = shm_open(MEM_NAME, O_CREAT | O_RDONLY, 0777);

    int *array = (int *) mmap(0, ARRAY_SIZE * sizeof(int),
        PROT_READ, MAP_SHARED, mem_fd, 0);

    int *mov = array;

    sem_t* sem = sem_open(SEM_NAME, 0);

    int pid = (int) getpid();

    int i = 0;
    while(1) {

        sem_wait(sem);

        int j, n = 0;
        for (j = 0; j < ARRAY_SIZE; j++) {
            if (mov[j] == x) n++;
        }

        printf("(PID: %d\tTIMESTAMP: %u)\t"
            "Znalazlem %d liczb o wartosci %d.\n",
            pid, (unsigned) time(NULL), n, x);

        sem_post(sem);
        i++;

        if (i % 5 == 4) {
            sleep(4);
            i = 0;
        } else {
            sleep(2);
        }
    }
}
