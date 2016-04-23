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
int MY_ID = -1;
int READY = 1;

void close_queue() {
	if (ID >= 0) {
		msgctl(ID, IPC_RMID, NULL);
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

	if (num < 2) is_primary = 0;

	struct q_msg buf;
	buf.type = 3;

	if (is_primary) {
		sprintf(buf.msg, "YPrimary number: %d (client: %d)", num, MY_ID);
	} else {
		sprintf(buf.msg, "NPrimary number: %d (client: %d)", num, MY_ID);
	}

	msgsnd(ID, &buf, sizeof(struct q_msg) - sizeof(long), 0);

	READY = 1;
}

void send_request() {
	struct q_msg buf;
	buf.type = 2;
	sprintf(buf.msg, "%d", MY_ID);

	printf("Sending request to server, my id: %s\n", buf.msg);

	msgsnd(ID, &buf, sizeof(struct q_msg) - sizeof(long), 0);
}

int main(int argc, char **argv) {
	srand(time(NULL));
	struct command_args args = get_args(argc, argv);
	atexit(close_queue);

	key_t key_s;
	int server_id;

	ID = msgget(IPC_PRIVATE, IPC_CREAT | 0700);
	check_failure(ID, -1, "failed to execute msgget [1]\n");

	key_s = ftok(args.path, args.id);
	check_failure((int) key_s, -1, "failed to execute ftok\n");

	server_id = msgget(key_s, 0700);
	check_failure(server_id, -1, "failed to execute msgget [2]\n");

	struct q_msg buf;
	buf.type = 1;
	sprintf(buf.msg, "%d", ID);

	msgsnd(server_id, &buf, sizeof(struct q_msg) - sizeof(long), 0);

	while(1) {

		int status;

		status = msgrcv(ID, &buf, sizeof(struct q_msg) - sizeof(long), 6, IPC_NOWAIT);

		if (status > 0) {
			printf("Communication started.\n");
			MY_ID = atoi(buf.msg);
		}

		status = msgrcv(ID, &buf, sizeof(struct q_msg) - sizeof(long), 7, IPC_NOWAIT);

		if (status > 0) {
			check_num(buf.msg);
		}

		if (READY == 1 && MY_ID >= 0) {
			READY = 0;
			send_request();
		}

		sleep(2);
	}

	return 0;
}