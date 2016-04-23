#ifndef MY_FUNCTIONS
#define MY_FUNCTIONS

struct command_args {
	char *path;
	int q_fd;
};

void check_failure(int val, int err_val, char *msg);

struct command_args get_args(int argc, char **argv);

#endif