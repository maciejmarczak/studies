#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

typedef struct {
    char* filename;
    int recordSize;
    int numOfRecords;
} CommandArgs;

CommandArgs* getCommandArgs(int argc, char** argv) {
    if(argc != 4) return NULL;

    CommandArgs* args = malloc(sizeof(CommandArgs));

    args->filename = argv[1];
    args->recordSize = atoi(argv[2]);
    args->numOfRecords = atoi(argv[3]);

    if(args->recordSize == 0 || args->numOfRecords == 0) {
        free(args);
        return NULL;
    }

    return args;
}

char* generateLine(int recordSize) {
    char* line = malloc(recordSize * sizeof(char));

    int i;
    for(i = 0; i < recordSize - 1; i++) {
        *(line + i) = (char) (97 + rand() % 26);
    }

    *(line + recordSize - 1) = '\n';

    return line;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    CommandArgs* args = getCommandArgs(argc, argv);

    if(args == NULL) {
        printf("Wrong command line arguments.\n");
        return 0;
    }

    int fileDescriptor = open(args->filename, O_RDWR | O_CREAT, 0777);

    int i;
    for(i = 0; i < args->numOfRecords; i++) {
        char* line = generateLine(args->recordSize);
        write(fileDescriptor, line, args->recordSize);
        free(line);
    }

    close(fileDescriptor);

    return 0;
}
