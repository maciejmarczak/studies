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

	int fifo;
	check_failure((fifo = open(argv[1], O_WRONLY)), -1, "couldn't open fifo\n");

	char buffer[BUFSIZE];
	char message[2 * BUFSIZE];
	int exit = 0;

	char pid_num[10];
	sprintf(pid_num, "%ld ", (long) getpid());

	time_t t;
	char *curr_time;

	do {

		fgets(buffer, BUFSIZE, stdin);

		if (strlen(buffer) == 5 && strcmp(buffer, "exit\n") == 0) {
			exit = 1;
		}

		t = time(NULL);
		curr_time = ctime(&t);
		curr_time[strlen(curr_time) - 1] = ' ';

		message[0] = 0;

		strcat(message, "PID: ");
		strcat(message, pid_num);
		strcat(message, "\tSAVE TIME: ");
		strcat(message, curr_time);
		strcat(message, "\tMESSAGE: ");
		strcat(message, buffer);

		check_failure(write(fifo, exit ? buffer : message, 2 * BUFSIZE), -1, "failed to write to fifo\n");

	} while (!exit);

	check_failure(close(fifo), -1, "failed to close fifo\n");

	return 0;
}