#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int SIGNALS_RECEIVED = 0;
pid_t CALLING_PID;

void signal_received(int sig, siginfo_t *info, void *context) {
	SIGNALS_RECEIVED++;

	if(SIGNALS_RECEIVED == 1) {
		CALLING_PID = info->si_pid;
	}

	kill(CALLING_PID, SIGUSR1);
}

void send_signals(int sig) {
	printf("SIGNALS_RECEIVED: %d\n", SIGNALS_RECEIVED);

	int i;
	for(i = 0; i < SIGNALS_RECEIVED; i++) {
		kill(CALLING_PID, SIGUSR1);
	}

	kill(CALLING_PID, SIGUSR2);
	exit(0);
}

int main() {
	struct sigaction act;
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &signal_received;

	sigaction(SIGUSR1, &act, NULL);
	signal(SIGUSR2, send_signals);

	while(1) {
		
	}

	return 0;
}