#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int SIGNALS_RECEIVED = 0;
int NUM_OF_SIGNALS;
int INC = 0;
pid_t PID;

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
	if(++INC <= NUM_OF_SIGNALS) {
		kill(PID, SIGUSR1);
	}
	else {
		kill(PID, SIGUSR2);
	}
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
	PID = strtoul(line, NULL, 10);

	pclose(cmd);

	signal(SIGUSR1, signal_received);
	signal(SIGUSR2, finish_receiving);

	INC++;
	kill(PID, SIGUSR1);

	while(1) {

	}

	return 0;
}