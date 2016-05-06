#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>

#define T_COUNT 2

static pthread_t threads[T_COUNT];

void assert(int cond, const char* message);
void* task(void* args);
void* zero_task(void* args);

int main(int argc, char** argv) {
    
    int i;
    int res;
    for(i = 0; i < T_COUNT; i++) {
	res = pthread_create(threads + i, NULL, task, NULL);
	assert(res == 0, "Error when spawning thread");
    }

    pthread_t z_thread;
    pthread_create(&z_thread, NULL, zero_task, NULL);

    for(i = 0; i < T_COUNT; i++) {
	pthread_join(threads[i], NULL);
    }

    printf("Finishing..\n");
    exit(EXIT_FAILURE);
}

void assert(int cond, const char* message) {
    if(!cond) {
	perror(message);
    }
}

void* task(void* arg) {
    
    pthread_t self = pthread_self();

    while(1) {
	printf("TID: %ld\n", (unsigned long)self);
	sleep(3);
    }
    
    pthread_exit(0);
}

void* zero_task(void* arg) {

    sleep(5);

    printf("%d", 1 / 0);

    pthread_exit(0);
}

