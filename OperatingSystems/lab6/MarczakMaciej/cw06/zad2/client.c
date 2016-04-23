#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "common_functions.h"
#include <unistd.h>
#include <time.h>
#include <mqueue.h>
#include <errno.h>
#include <signal.h>

int ID = -1, MY_ID = -1, READY = 1, server_id;
char name[50];

struct command_args args;

void close_queue() {
	if (ID >= 0) {
		mq_close(ID);
		mq_unlink(name);
	}
}

void check_num(char *msg) {
	printf("Checking whether number is primary.\n");
	int num = atoi(msg);
	int is_primary = 1;

	int i;
	for (i = 2; i < num; i++) {
		if (num % i == 0) {
			is_primary = 0;
			break;
		}
	}

	char response[100];

	if (num < 2) is_primary = 0;

	if (is_primary) {
		sprintf(response, "3YPrimary number: %d (client: %d)", num, MY_ID);
	} else {
		sprintf(response, "3NPrimary number: %d (client: %d)", num, MY_ID);
	}

	mq_send(server_id, response, 45, 0);
	sleep(2);

	READY = 1;
}

void send_request() {
	char msg[40];
	sprintf(msg, "2%c", MY_ID);

	printf("Sending request to server, my id: %s\n", msg);

	mq_send(server_id, msg, 30, 0);
}

int main(int argc, char **argv) {
	
	srand(time(NULL));
	signal(SIGINT, exit);
	atexit(close_queue);

	args = get_args(argc, argv);

	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 50;

	server_id = mq_open(args.path, O_WRONLY);
	check_failure(server_id, -1, "failed to connect to servers' queue\n");

	sprintf(name, "/client%d", rand() % 5000);

	ID = mq_open(name, O_RDWR | O_CREAT | O_NONBLOCK, 0777, &attr);
	check_failure(ID, -1, "failed to create clients' queue\n");

	printf("Sending message to server with value: %s\n", name);
	mq_send(server_id, name, 20, 0);

	char msg[100];

	while(1) {
		mq_receive(ID, msg, 100, NULL);
		if (msg[0] == '6') {
			printf("Communication started.\n");
			MY_ID = atoi(msg + 1);
			break;
		}
	}

	while(1) {		

		if (!READY) {
			mq_receive(ID, msg, 100, NULL);

			if (msg[0] == '7') {
				check_num(msg + 1);
			}
		}

		if (READY == 1 && MY_ID >= 0) {
			READY = 0;
			send_request();
		}

	}

	return 0;
}