#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "common_functions.h"
#include <unistd.h>
#include <time.h>
#include <mqueue.h>
#include <signal.h>

int MAX_RAND = 1000;
struct command_args args;

void close_queue() {
	if (args.q_fd >= 0) {
		mq_close(args.q_fd);
		mq_unlink(args.path);
	}
}

int clients[20];
int client_no = 0;

void start_communication(char *msg) {
	if (client_no == 19) return;

	clients[client_no] = mq_open(msg, O_RDWR | O_NONBLOCK);
	check_failure(clients[client_no], -1, "failed to create clients' queue\n");

	char response[10];
	sprintf(response, "6%d", client_no);
	printf("Starting communication with client no %d, his queue id: %d\n", client_no, clients[client_no]);

	mq_send(clients[client_no], response, 10, 0);

	client_no++;
}

void send_random(char *msg) {
	int client = (int) msg[0];
	if (client >= 20) return;

	char response[20];
	sprintf(response, "7%d", (rand() % MAX_RAND));

	//printf("Sending random number to client, number generated: %s\n", response);

	mq_send(clients[client], response, 20, 0);
}

void process_response(char *msg) {
	//printf("Processing response.\n");
	if (msg[0] == 'Y') {
		printf("%s\n", msg + 1);
	}
}

int main(int argc, char **argv) {
	srand(time(NULL));

	signal(SIGINT, exit);

	args = get_args(argc, argv);
	atexit(close_queue);

	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 50;

	args.q_fd = (int) mq_open(args.path, O_RDONLY | O_CREAT, 0777, &attr);
	check_failure(args.q_fd, -1, "failed to create queue\n");

	char msg[100];

	while(1) {

		int status = mq_receive(args.q_fd, msg, 100, NULL);

		if (status > 0) {
			if (msg[0] == '2') send_random(msg + 1);
			else if (msg[0] == '3') process_response(msg + 1);
			else if (msg[0] == '/') start_communication(msg);
		}

	}

	return 0;
}