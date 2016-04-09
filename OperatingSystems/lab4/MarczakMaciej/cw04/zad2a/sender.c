#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int SIGNALS_RECEIVED = 0;
int NUM_OF_SIGNALS;

// temporary version of function, to be upgraded
void get_command_args(int argc, char **argv) {
	if(argc != 2) {
		NUM_OF_SIGNALS = 1000;
		printf("Number of signals set to default value of 1000.\n");
		return;
	}

	NUM_OF_SIGNALS = atoi(argv[1]);
}

void signal_received(int sig) {
	SIGNALS_RECEIVED++;
}

void finish_receiving(int sig) {
	printf("Expected signals: %d | Received signals: %d\n", NUM_OF_SIGNALS, SIGNALS_RECEIVED);
	exit(0);
}

int main(int argc, char **argv) {
	get_command_args(argc, argv);

	char line[10];
	FILE *cmd = popen("pidof -s catcher", "r");

	fgets(line, 10, cmd);
	pid_t pid = strtoul(line, NULL, 10);

	pclose(cmd);

	signal(SIGUSR1, signal_received);
	signal(SIGUSR2, finish_receiving);

	int i;
	for(i = 0; i < NUM_OF_SIGNALS; i++) {
		kill(pid, SIGUSR1);
	}

	kill(pid, SIGUSR2);

	while(1) {

	}

	return 0;
}