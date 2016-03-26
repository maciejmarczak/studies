#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

// starting with signal no = 1
int CURRENT_SIGNAL = 1;
int INCREMENT = -1;

// calculadet by get_command_arguments
int TOP_SIGNAL_NUM;

typedef struct {
	char *text;
	char *reversed_text;
	int max_num;
} args;

args get_command_arguments(int argc, char **argv) {
	if(argc != 3) {
		printf("There should be two command line arguments.\n");
		exit(-1);
	}

	int max_num = atoi(argv[2]);

	if(max_num < 2) {
		printf("Wrong max_num argument.\n");
		exit(-1);
	}

	args command_args;

	command_args.text = argv[1];
	command_args.max_num = max_num;

	// reversing text
	int text_length = (int) strlen(argv[1]);
	command_args.reversed_text = malloc((text_length + 1) * sizeof(char));

	int i;
	for(i = 0; i < text_length; i++) {
		command_args.reversed_text[i] = command_args.text[text_length - i - 1];
	}

	command_args.reversed_text[i] = '\0';
	// after reversing

	TOP_SIGNAL_NUM = 3 * command_args.max_num;

	return command_args;
}

void print_text(args command_args) {
	int flag_reversed = (CURRENT_SIGNAL - 2) % 3 == 0 ? 1 : 0;
	char *to_print = flag_reversed ? command_args.reversed_text : command_args.text;

	int how_many_times;
	switch(CURRENT_SIGNAL % 3) {
		case 1:
			how_many_times = (CURRENT_SIGNAL + 2) / 3;
			break;
		case 2:
			how_many_times = (CURRENT_SIGNAL + 1) / 3;
			break;
		case 0:
			how_many_times = (CURRENT_SIGNAL) / 3;
	}

	int i;
	for(i = 0; i < how_many_times; i++) {
		printf("%s\n", to_print);
	}

	printf("\n\n\n");
}

void increment_global(int sig) {
	if(CURRENT_SIGNAL == 1 || CURRENT_SIGNAL == TOP_SIGNAL_NUM) {
		INCREMENT *= -1;
	}

	CURRENT_SIGNAL += INCREMENT;
}

void exit_program(int sig) {
	printf("Odebrano sygnał SIGINT\n");
	exit(0);
}

int main(int argc, char **argv) {
	args command_args = get_command_arguments(argc, argv);

	struct sigaction act;
	act.sa_handler = &increment_global;

	sigaction(SIGTSTP, &act, NULL);

	signal(SIGINT, exit_program);

	while(1) {
		print_text(command_args);
		sleep(3);
	}

	return 0;
}