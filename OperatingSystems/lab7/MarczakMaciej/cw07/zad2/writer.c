#include "headers.h"

int main() {
    srandom(time(NULL));

    int mem_fd = shm_open(MEM_NAME, O_CREAT | O_RDWR, 0777);
    int *array = (int *) mmap(0, ARRAY_SIZE * sizeof(int),
        PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0);

    int *mov = array;

    sem_t* sem = sem_open(SEM_NAME, 0);

    int init_val;

    int pid = (int) getpid();

    int i = 0;
    while(1) {

        sem_wait(sem);
        init_val = random() % MAX_RAND;

        int j, k, r = rand() % (ARRAY_SIZE / 3);
        for (j = 0; j < r; j++) {
            k = rand() % ARRAY_SIZE;
            mov[k] = init_val++;

            printf("(PID: %d\tTIMESTAMP: %u)\t"
                "Wpisalem liczbe %d na pozycje %d. "
                "Pozostalo %d zadan.\n",
                pid, (unsigned) time(NULL), mov[k], k, r - 1 - j);
        }

        sem_post(sem);
        i++;

        if (i % 5 == 4) {
            sleep(2);
            i = 0;
        } else {
            sleep(0.5);
        }
    }
}
