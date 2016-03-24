#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <sched.h>

#define STACK_SIZE (1024 * 1024)
int GLOBAL_COUNTER = 0;
double CHILDREN_REAL;

typedef struct {
	int N;
	char *method;
	int methodID;
} CommandArgs;

int isMethodName(char *method) {
	if(strcmp(method, "fork") == 0) return 1;
	if(strcmp(method, "vfork") == 0) return 2;
	if(strcmp(method, "clone") == 0) return 3;
	if(strcmp(method, "vclone") == 0) return 4;

	return 0;
}

void printTimes(int who) {
	clock_t myClock = clock();

	struct rusage rusage;
	getrusage(who, &rusage);

	double realTime = (who == RUSAGE_SELF) ? (double) myClock / CLOCKS_PER_SEC : CHILDREN_REAL;
	double userTime = (double) rusage.ru_utime.tv_sec + (double) rusage.ru_utime.tv_usec / 10e6;
	double sysTime = (double) rusage.ru_stime.tv_sec + (double) rusage.ru_stime.tv_usec / 10e6;

	printf("\n- real time: %12.1f s", realTime);
	printf("\n- user time: %12.1f s", userTime);
	printf("\n- syst time: %12.1f s\n\n", sysTime);

}

int childTask(void *args) {
	GLOBAL_COUNTER++;
	_exit(EXIT_SUCCESS);
}

pid_t newChild(int methodID) {
	switch(methodID) {
		case 1:
			return fork();
		case 2:
			return vfork();
		case 3:
		case 4:
			{
				char *stack, *stackTop;

				stack = malloc(STACK_SIZE * sizeof(char));
				stackTop = stack + STACK_SIZE;

				return clone(childTask, stackTop,
					(methodID == 3) ? SIGCHLD : CLONE_VM | CLONE_VFORK | SIGCHLD, NULL);
			}
	}
}

CommandArgs getCommandArgs(int argc, char **argv) {
	CommandArgs args;

	args.N = 400000;
	args.method = "fork";
	args.methodID = 1;

	int passedN, passedMethodID;
	char *passedMethod;

	if(argc != 3 || (passedN = atoi(argv[1])) == 0 || !(passedMethodID = isMethodName(passedMethod = argv[2]))) {
		printf("\nWrong command arguments. Default values set: N = %d and method = %s.\n\n", args.N, args.method);
		return args;
	}

	args.N = passedN;
	args.method = passedMethod;
	args.methodID = passedMethodID;

	return args;
}

void spawnProcesses(CommandArgs args) {

	int status;

	struct timeval t_before, t_after;
	struct timezone tz;

	gettimeofday(&t_before, &tz);
	int i;
	for(i = 0; i < args.N; i++) {
		pid_t pid = newChild(args.methodID);

		if(pid < 0) {
			waitpid(-1, &status, 0);
			continue;
		}

		if(pid == 0) {
			childTask(NULL);
		}
	}

	while(waitpid(-1, &status, 0) > 0);
	gettimeofday(&t_after, &tz);

	printf("Back in parent process. GLOBAL_COUNTER = %d\n", GLOBAL_COUNTER);
	CHILDREN_REAL = (double) (t_after.tv_sec - t_before.tv_sec);
}

int main(int argc, char **argv) {

	spawnProcesses(getCommandArgs(argc, argv));
	
	printf("Parent process:\n");
	printTimes(RUSAGE_SELF);
	
	printf("Child processes:\n");
	printTimes(RUSAGE_CHILDREN);

	return 0;
}