#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

int BUFSIZE = 512;

void check_failure(int val, int error, char *message) {
	if (val == error) {
		perror(message);
		exit(-1);
	}
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("usage: [program name] [1st argument: fifo name]\n");
		exit(-1);
	}

	if (strlen(argv[1]) > 10 || strlen(argv[1]) < 3) {
		printf("fifo name should be between 3 and 10 characters long\n");
		exit(-1);
	}

	check_failure(mkfifo(argv[1], S_IRWXU | S_IRWXG | S_IRWXO), -1, "failed to create fifo\n");

	int fifo;
	check_failure((fifo = open(argv[1], O_RDONLY)), -1, "couldn't open fifo\n");

	char buffer[2 * BUFSIZE];
	char *curr_time;

	int exit = 0;

	do {

		check_failure(read(fifo, buffer, 2 * BUFSIZE), -1, "failed to read from fifo\n");

		time_t t = time(NULL);
		curr_time = ctime(&t);

		if (strlen(buffer) == 5 && strcmp(buffer, "exit\n") == 0) {
			exit = 1;
		}

		curr_time[strlen(curr_time) - 1] = ' ';

		printf("READ TIME: %s\t%s\n", curr_time, buffer);

	} while (!exit);

	check_failure(close(fifo), -1, "failed to close fifo\n");
	check_failure(unlink(argv[1]), -1, "failed while executing unlink on fifo\n");

	return 0;
}