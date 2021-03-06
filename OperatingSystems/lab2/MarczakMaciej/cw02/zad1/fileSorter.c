#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/times.h>
#include <time.h>
#include <sys/resource.h>

typedef struct {
    char* filename;
    int recordSize;
    char* mode;
} CommandArgs;

CommandArgs* getCommandArgs(int argc, char** argv) {
    if(argc != 4) return NULL;

    CommandArgs* args = malloc(sizeof(CommandArgs));

    args->filename = argv[1];
    args->recordSize = atoi(argv[2]);
    args->mode = argv[3];

    if(args->recordSize == 0 || (strcmp("lib", args->mode) != 0 && strcmp("sys", args->mode) != 0)) {
        free(args);
        return NULL;
    }

    return args;
}

int compare(char* string1, char* string2) {
    int a = (int) string1[0];
    int b = (int) string2[0];

    if(a >= b) return 1;

    return -1;
}

void sortUsingSys(CommandArgs* args) {

    printf("\nSORTING USING SYSTEM LIBRARY\n");

    int rs = args->recordSize;
    int fileDescriptor = open(args->filename, O_RDWR);

    if(fileDescriptor == -1) {
        printf("Couldn't open given file.\n");
        return;
    }

    char* currentlyInserted = malloc(rs * sizeof(char));
    char* compared = malloc(rs * sizeof(char));

    off_t currPos = lseek(fileDescriptor, rs, SEEK_SET);
    int status = read(fileDescriptor, currentlyInserted, rs);

    while(status != 0) {
        
        off_t i;
        for(i = currPos - rs; i >= 0; i -= rs) {

            i = lseek(fileDescriptor, i, SEEK_SET);
            read(fileDescriptor, compared, rs);

            if(compare(compared, currentlyInserted) > 0) {
                lseek(fileDescriptor, i, SEEK_SET);
                write(fileDescriptor, currentlyInserted, rs);
                write(fileDescriptor, compared, rs);
            } else {
                break;
            }
        }

        currPos = lseek(fileDescriptor, currPos + rs, SEEK_SET);
        status = read(fileDescriptor, currentlyInserted, rs);
    }

    free(currentlyInserted);
    free(compared);
    close(fileDescriptor);
}

void sortUsingLib(CommandArgs* args) {

    printf("\nSORTING USING C LIBRARY\n");

    int rs = args->recordSize;
    FILE* file = fopen(args->filename, "r+");
    
    if(file == NULL) {
        printf("Couldn't open the file.\n");
        return;
    }

    char* currentlyInserted = malloc(rs * sizeof(char));
    char* compared = malloc(rs * sizeof(char));

    fseek(file, rs, SEEK_SET);
    int currPos = rs;
    int status = fread(currentlyInserted, sizeof(char), rs, file);

    while(status != 0) {
        
        int i;
        for(i = currPos - rs; i >= 0; i -= rs) {

            fseek(file, i, SEEK_SET);
            fread(compared, sizeof(char), rs, file);

            if(compare(compared, currentlyInserted) > 0) {
                fseek(file, i, SEEK_SET);
                fwrite(currentlyInserted, sizeof(char), rs, file);
                fwrite(compared, sizeof(char), rs, file);
            } else {
                break;
            }
        }

        currPos = currPos + rs;
        fseek(file, currPos, SEEK_SET);
        status = fread(currentlyInserted, sizeof(char), rs, file);
    }

    free(currentlyInserted);
    free(compared);
    fclose(file);
}

void printTimes() {
    struct rusage rusage;
    getrusage(RUSAGE_SELF, &rusage);

    double userTime = (double) rusage.ru_utime.tv_sec + (double) rusage.ru_utime.tv_usec / 10e6;
    double sysTime = (double) rusage.ru_stime.tv_sec + (double) rusage.ru_stime.tv_usec / 10e6;

    printf("\n- user time: %10.9f s", userTime);
    printf("\n- syst time: %10.9f s", sysTime);

    clock_t clck = clock();

    double realTime = (double) clck / CLOCKS_PER_SEC;

    printf("\n- real time: %10.9f s\n\n\n", realTime);
}

int main(int argc, char* argv[]) {
    CommandArgs* args = getCommandArgs(argc, argv);

    if(args == NULL) {
        printf("Wrong command line arguments.\n");
        return 0;
    }

    if(strcmp("sys", args->mode) == 0) sortUsingSys(args);
    if(strcmp("lib", args->mode) == 0) sortUsingLib(args);

    printTimes();

    return 0;
}
