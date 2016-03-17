#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct lock {
    struct lock *next;
    off_t byte;
    int type;
} lock;

typedef struct {
    struct lock *head;
} lockList;

void addLock(lockList *ll, off_t byte, int type) {
    lock *tmp = malloc(sizeof(struct lock));

    tmp->next = ll->head;
    tmp->byte = byte;
    tmp->type = type;

    ll->head = tmp;
}

lock *findLock(lockList *ll, off_t byte) {
    lock *tmp = ll->head;

    while(tmp != NULL && tmp->byte != byte) {
        tmp = tmp->next;
    }

    return tmp;
}

void removeLock(lockList *ll, off_t byte) {
    lock *prev, *curr;

    prev = NULL;
    curr = ll->head;

    while(curr != NULL && curr->byte != byte) {
        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL) {
        return;
    }

    if(prev == NULL) {
        ll->head = ll->head->next;
        free(curr);
        return;
    }

    prev->next = curr->next;
    free(curr);
}

int getArgument(char *argument) {
    return atoi(argument);
}

void setLock(int filedes, int cmd, short l_type, off_t l_start, lockList *ll) {
    struct flock fptr;

    fptr.l_type = l_type;
    fptr.l_start = l_start - 1;
    fptr.l_whence = SEEK_SET;
    fptr.l_len = 1;

    lock *tmp = findLock(ll, l_start);

    if(tmp != NULL) {
        if(tmp->type == l_type) {
            return;
        }

        if(l_type == F_UNLCK) {
            removeLock(ll, tmp->byte);
            fcntl(filedes, F_SETLK, &fptr);
            printf("Released lock at byte number %zu.\n", l_start);
            return;
        }

        if(tmp->type == F_WRLCK) {
            removeLock(ll, tmp->byte);
            addLock(ll, l_start, l_type);
            fcntl(filedes, F_SETLK, &fptr);
            printf("Changed lock from WRITE mode to READ at by number %zu.\n", l_start);
            return;
        }

        if(tmp->type == F_RDLCK) {
            if(fcntl(filedes, F_SETLK, &fptr) != -1) {
                removeLock(ll, tmp->byte);
                addLock(ll, l_start, l_type);
                printf("Changed lock from READ mode to WRITE at by number %zu.\n", l_start);
                return;
            }
        }
    }

    if(fcntl(filedes, F_SETLK, &fptr) != -1) {
        printf("Set lock at byte number %zu.\n", l_start);
        addLock(ll, l_start, l_type);
    }
}

void printLocks(int filedes, lockList *ll) {

    lock *tmp;
    tmp = ll->head;

    while(tmp != NULL) {
        struct flock fptr;

        fptr.l_type = tmp->type;
        fptr.l_start = tmp->byte - 1;
        fptr.l_whence = SEEK_SET;
        fptr.l_len = 1;
        fptr.l_pid = getpid();

        fcntl(filedes, F_GETLK, &fptr);

        printf("BYTE AT: %zu, LOCKED: %d, by PID: %d\n", tmp->byte, tmp->type, fptr.l_pid);

        tmp = tmp->next;
    }

    int i = 0;
    char buf[1];
    while(pread(filedes, buf, 1, i) > 0) {
        struct flock fptr;

        fptr.l_type = F_WRLCK;
        fptr.l_start = i++;
        fptr.l_whence = SEEK_SET;
        fptr.l_len = 1;

        fcntl(filedes, F_GETLK, &fptr);

        if(fptr.l_type != F_UNLCK) {
            printf("BYTE AT: %zu, LOCKED: %d, by PID: %d\n", fptr.l_start + 1, fptr.l_type, fptr.l_pid);
        }
    }

}

char getCharAt(int filedes, int byteNumber) {
    struct flock fptr;

    fptr.l_type = F_WRLCK;
    fptr.l_start = byteNumber - 1;
    fptr.l_whence = SEEK_SET;
    fptr.l_len = 1;

    fcntl(filedes, F_GETLK, &fptr);

    if(fptr.l_type == F_WRLCK) {
        printf("WRITE block on that char by another process.\n");
        return ' ';
    }

    char result[1];

    pread(filedes, result, 1, byteNumber - 1);

    return result[0];
}

void replaceCharAt(int filedes, int byteNumber, char c) {
    struct flock fptr;

    fptr.l_type = F_WRLCK;
    fptr.l_start = byteNumber - 1;
    fptr.l_whence = SEEK_SET;
    fptr.l_len = 1;

    fcntl(filedes, F_GETLK, &fptr);

    if(fptr.l_type == F_WRLCK || fptr.l_type == F_RDLCK) {
        printf("Block on that char by another process.\n");
        return;
    }

    char result[2] = { c, '\0' };

    pwrite(filedes, result, 1, byteNumber - 1);

    return;
}

int main(int argc, char **argv) {

    if(argc != 2) {
        printf("Wrong number of arguments.\n");
        return;
    }

    char *filename = argv[1];

    int fd;
    if((fd = open(filename, O_RDWR)) == -1) {
        printf("Couldn't open the file.\n");
        return;
    }

    lockList *ll = malloc(sizeof(lockList));
    ll->head = NULL;

    const int MAXLEN = 15;
    char command[MAXLEN], ch;

    while(fgets(command, MAXLEN, stdin)) {

        command[strlen(command) - 1] = '\0';
        int byteNumber;

        if(strcmp("help", command) == 0) {
            printf("\n\nType in:\n");
            printf("q - for quitting.\n");
            printf("slrX - SET LOCK for READING on byte number X.\n");
            printf("slwX - SET LOCK for WRITING on byte number X.\n");
            printf("rllX - RELEASE LOCK on byte number X.\n");
            printf("show - SHOW all locks in current file.\n");
            printf("rX - READ byte number X.\n");
            printf("wY-X - OVERRIDE byte number X with byte Y.\n\n");
        }

        else if(strncmp("slr", command, 3) == 0) {
            byteNumber = getArgument(command + 3);

            setLock(fd, F_SETLK, F_RDLCK, byteNumber, ll);
        }

        else if(strncmp("slw", command, 3) == 0) {
            byteNumber = getArgument(command + 3);

            setLock(fd, F_SETLK, F_WRLCK, byteNumber, ll);
        }

        else if(strncmp("rll", command, 3) == 0) {
            byteNumber = getArgument(command + 3);

            setLock(fd, F_SETLK, F_UNLCK, byteNumber, ll);
        }

        else if(strncmp("show", command, 4) == 0) {
            printLocks(fd, ll);
        }

        else if(strncmp("r", command, 1) == 0) {
            byteNumber = getArgument(command + 1);

            printf("%c\n", getCharAt(fd, byteNumber));
        }

        else if(strncmp("w", command, 1) == 0) {
            if(command[2] != '-') {
                continue;
            }

            byteNumber = getArgument(command + 3);
            char y = command[1];

            replaceCharAt(fd, byteNumber, y);
        }

        else if(strncmp("q", command, 1) == 0) {
            break;
        }
    }

    close(fd);

    return 0;
}
