#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define DEFAULT_N "5"

char *get_command_arg(int argc, char **argv) {
	
	char *fold_arg = DEFAULT_N;

	if (argc == 2) {
		fold_arg = ( atoi(argv[1]) > 0 ) ? argv[1] : DEFAULT_N;
	}

	return fold_arg;

}

void check_failure(int val, int error, char *message) {
	if (val == error) {
		perror(message);
		exit(-1);
	}
}

int main(int argc, char **argv) {

	char *fold_arg = get_command_arg(argc, argv);

	pid_t pid;
	int fd[2];

	if ( (pipe(fd)) != 0 ) {
		perror("failed to execute pipe()\n");
		exit(-1);
	}

	if ( (pid = fork()) < 0 ) {
		perror("failed to execute fork()\n");
		exit(-1);
	}

	if (pid == 0) {
		check_failure(dup2(fd[1], STDOUT_FILENO), -1, "failed to execute dup2()");
		check_failure(close(fd[0]), -1, "failed to close file descriptor");
		check_failure(close(fd[1]), -1, "failed to close file descriptor");
		check_failure(execl("/usr/bin/tr", "tr", "'[:lower:]'", "'[:upper:]'", NULL), -1, "failed to execute execl()");
	} else {
		check_failure(dup2(fd[0], STDIN_FILENO), -1, "failed to execute dup2()");
		check_failure(close(fd[0]), -1, "failed to close file descriptor");
		check_failure(close(fd[1]), -1, "failed to close file descriptor");
		check_failure(execl("/usr/bin/fold", "fold", "-w", fold_arg, NULL), -1, "failed to execute execl()");
	}

	return 0;

}