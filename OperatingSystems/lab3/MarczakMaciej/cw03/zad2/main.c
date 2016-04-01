#define _XOPEN_SOURCE 500
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

int TEST_MODE = 0;
int PRINT_DIRS = 1;

char *concatenateStrings(char* s1, char* s2) {
	int totalSize = strlen(s1) + strlen(s2);

	char* result = malloc((totalSize + 2) * sizeof(char));
	memset(result, '\0', totalSize);

	strncat(result, s1, strlen(s1));
	strncat(result, "/", 1);
	strncat(result, s2, strlen(s2));

	return result;
}

int get_num_of_files(char *path, char **argv, int argc) {
	DIR* dir;

	if((dir = opendir(path)) == NULL) {
		printf("Couldn't open the dir.\n");
		return 0;
	}

	int num_of_files = 0;
	int num_of_dirs = 0;

	struct dirent *dnt;
	char *new_path;
	struct stat *buf;

	while((dnt = readdir(dir)) != NULL) {

		new_path = concatenateStrings(path, dnt->d_name);
		buf = malloc(sizeof(struct stat));

		stat(new_path, buf);
		if(S_ISREG(buf->st_mode)) {
			num_of_files++;
		}
		else if(S_ISDIR(buf->st_mode) && !(strcmp(dnt->d_name, ".") == 0 || strcmp(dnt->d_name, "..") == 0)) {
			num_of_dirs++;
			pid_t pid = fork();

			if(pid < 0) {
				printf("Couldn't spawn new process.\n");
				_exit(0);
			}

			if(pid == 0) {
				execl(argv[0], argv[0], new_path, argc >= 3 ? argv[2] : NULL, argc == 4 ? argv[3] : NULL, NULL);
			}
		}

		free(buf);
		free(new_path);
	}

	int status, subfolder_files = 0;

	int i;
	for(i = 0; i < num_of_dirs; i++) {
		wait(&status);
		subfolder_files += WEXITSTATUS(status);
	}

	closedir(dir);

	if(TEST_MODE) {
		sleep(15);
	}

	if(PRINT_DIRS) {
		printf("Number of files only inside %s directory is %d\n", path, num_of_files);
		printf("Total number of files inside %s tree is: %d\n", path, num_of_files + subfolder_files);
	}

	_exit(num_of_files + subfolder_files);
}

int main(int argc, char **argv) {

	if(!(argc == 2 || argc == 3 || argc == 4)) {
		printf("Wrong number of command line arguments.\n");
		exit(0);
	}

	char *path = argv[1];

	if(argc == 3) {
		int fails = 0;

		(strcmp(argv[2], "-w") == 0) ? TEST_MODE = 1 : fails++;
		(strcmp(argv[2], "-v") == 0) ? PRINT_DIRS = 0 : fails++;

		if(fails == 2) {
			printf("Wrong arguments!\n");
			exit(0);
		}
	}
	else if(argc == 4) {
		int fails = 0;

		(strcmp(argv[2], "-w") == 0) ? TEST_MODE = 1 : fails++;
		(strcmp(argv[3], "-v") == 0) ? PRINT_DIRS = 0 : fails++;

		if(fails != 0) {
			printf("Wrong arguments!\n");
			exit(0);
		}
	}

	get_num_of_files(path, argv, argc);

	return 1;
}