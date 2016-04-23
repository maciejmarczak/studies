#include "common_functions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void check_failure(int val, int err_val, char *msg) {
	if (val == err_val) {
		fprintf(stderr, "%s\n%s", msg, strerror(errno));
		sleep(5);
		exit(-1);
	}
}

struct command_args get_args(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "usage: ./program_name queue_path\n");
		exit(-1);
	}

	struct command_args args;
	args.path = argv[1];
	args.q_fd = -2;

	return args;
}