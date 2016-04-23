#ifndef MY_FUNCTIONS
#define MY_FUNCTIONS

struct q_msg {
	long type;
	char msg[100];
};

struct command_args {
	char *path;
	char id;
};

void check_failure(int val, int err_val, char *msg);

struct command_args get_args(int argc, char **argv);

#endif