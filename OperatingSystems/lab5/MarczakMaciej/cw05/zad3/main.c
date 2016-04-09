#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv) {
	
	if (argc != 2) {
		printf("usage: [program name] [1st arg: output file name]\n");
		exit(-1);
	}

	if (strlen(argv[1]) > 10) {
		printf("maximum output file length is 10 characters\n");
		exit(-1);
	}

	FILE *fpin, *fpout;
	int MAXLINE = 256;

	char line[MAXLINE];

	char command[20] = "grep ^d > ";

	if ( (fpin = popen("ls -l", "r")) == NULL ) {
		perror("failed to execute popen()\n");
		exit(-1);
	}

	if ( (fpout = popen(strcat(command, argv[1]), "w")) == NULL ) {
		perror("failed to execute popen()\n");
		exit(-1);
	}

	while (fgets(line, MAXLINE, fpin)) {
		fputs(line, fpout);
	}

	return 0;
}