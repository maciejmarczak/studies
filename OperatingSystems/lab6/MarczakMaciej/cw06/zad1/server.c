#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include "common_functions.h"
#include <unistd.h>
#include <time.h>

int ID = -2;
int MAX_RAND = 1000;

void close_queue() {
	if (ID >= 0) {
		msgctl(ID, IPC_RMID, NULL);
	}
}

int size = sizeof(struct q_msg) - sizeof(long);
int clients[20];
int client_no = 0;

void start_communication(char *msg) {
	if (client_no == 19) return;

	clients[client_no] = atoi(msg);
	check_failure(clients[client_no], -1, "failed to create clients' queue\n");

	struct q_msg buf;
	buf.type = 6;
	sprintf(buf.msg, "%d", client_no);
	//printf("Starting communication with client no %d, his queue id: %d\n", client_no, clients[client_no]);

	msgsnd(clients[client_no], &buf, size, 0);

	client_no++;
}

void send_random(char *msg) {
	int client = atoi(msg);
	if (client >= 20) return;

	//printf("Receiving request from client: %d\n", client);

	struct q_msg buf;
	buf.type = 7;
	sprintf(buf.msg, "%d", (rand() % MAX_RAND));

	//printf("Sending random number to client, number generated: %s\n", buf.msg);

	msgsnd(clients[client], &buf, size, 0);
}

void process_response(char *msg) {
	//printf("Processing response.\n");
	if (msg[0] == 'Y') {
		printf("%s\n", msg + 1);
	}
}

int main(int argc, char **argv) {
	srand(time(NULL));

	struct command_args args = get_args(argc, argv);
	atexit(close_queue);

	key_t key = ftok(args.path, args.id);
	check_failure((int) key, -1, "failed to execute ftok\n");

	ID = msgget(key, IPC_CREAT | 0700);
	check_failure(ID, -1, "failed to execute msgget\n");

	struct q_msg buf;

	while(1) {

		int status;

		status = msgrcv(ID, &buf, size, 1, IPC_NOWAIT);

		if (status > 0) {
			start_communication(buf.msg);
			continue;
		}

		int i;
		for (i = 0; i < client_no; i++) {

			status = msgrcv(clients[i], &buf, size, 2, IPC_NOWAIT);
			if (status > 0) {
				send_random(buf.msg);
			}

			status = msgrcv(clients[i], &buf, size, 3, IPC_NOWAIT);
			if (status > 0) {
				process_response(buf.msg);
			}
		}

	}

	return 0;
}