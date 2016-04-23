#ifndef HEADER_FILE
#define HEADER_FILE

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>

#define MAX_TASKS 10
#define MEM_PATH "."
#define MEM_ID 'm'
#define SEM_PATH "."
#define SEM_ID 's'
#define MAXRAND 5000

#endif
