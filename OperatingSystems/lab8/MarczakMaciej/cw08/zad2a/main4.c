#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#define T_COUNT 2
#define SIGNAL SIGTERM

static pthread_t threads[T_COUNT];

void assert(int cond, const char* message);

void* task(void* args);
void* killing_task(void* pid);

int main(int argc, char** argv) {

    pid_t self = getpid();
    
    int i;
    int res;
    for(i = 0; i < T_COUNT; i++) {
	res = pthread_create(threads + i, NULL, task, NULL);
	assert(res == 0, "Error when spawning thread");
    }

    pthread_t k_thread;
    pthread_create(&k_thread, NULL, killing_task, (void*)&self);

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
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGNAL);
    sigprocmask(SIG_SETMASK, &set, NULL);

    pthread_t self = pthread_self();

    while(1) {
	printf("TID: %ld\n", (unsigned long)self);
	sleep(3);
    }
    
    pthread_exit(0);
}

void* killing_task(void* pid) {
    pid_t main_pid = *((pid_t*)pid);

    printf("PID: %d\n", (int)main_pid);

    pthread_kill(threads[0], SIGNAL);

    pthread_exit(0);
}
