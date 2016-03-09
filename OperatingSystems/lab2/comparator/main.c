#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    int a = (int) *string1;
    int b = (int) *string2;

    if(a < b) return 1;

    return 0;
}

void swapLines(char* compared, char* currentlyInserted, int recordSize) {
    char tmp;

    int i;
    for(i = 0; i < recordSize; i++) {
        tmp = compared[i];
        compared[i] = currentlyInserted[i];
        currentlyInserted[i] = tmp;
    }
}

void sortUsingSys(CommandArgs* args) {

}

void sortUsingLib(CommandArgs* args) {

}

int main(int argc, char* argv[]) {
    CommandArgs* args = getCommandArgs(argc, argv);

    if(args == NULL) {
        printf("Wrong command line arguments.\n");
        return 0;
    }

    if(strcmp("sys", args->mode) == 0) sortUsingSys(args);
    if(strcmp("lib", args->mode) == 0) sortUsingLib(args);

    return 0;
}
