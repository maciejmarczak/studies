#include "common_functions.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void check_failure(int val, int err_val, char *msg) {
	if (val == err_val) {
		fprintf(stderr, "%s", msg);
		sleep(5);
		exit(-1);
	}
}

struct command_args get_args(int argc, char **argv) {
	if (argc < 3) {
		fprintf(stderr, "usage: ./program_name project_path project_id\n");
		exit(-1);
	}

	if (strlen(argv[2]) != 1) {
		fprintf(stderr, "project_id must be a single character\n");
		exit(-1);
	}

	struct command_args args;
	args.path = argv[1];
	args.id = argv[2][0];

	return args;
}