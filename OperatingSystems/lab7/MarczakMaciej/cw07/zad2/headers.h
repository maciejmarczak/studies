#ifndef HEADER_FILE
#define HEADER_FILE

#define _POSIX_SOURCE
#define _BSD_SOURCE
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>

#define MEM_NAME "/smem"
#define SEM_NAME "/ssem"
#define ARRAY_SIZE 30
#define MAX_RAND 10

#endif
