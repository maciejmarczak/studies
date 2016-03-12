#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <ftw.h>
#include <stdint.h>

typedef struct {

	char* path;
	char* rights;

} CommandArgs;

char gRights[150];

CommandArgs* getCommandArgs(int argc, char** argv) {
	if(argc != 3 || strlen(argv[2]) != 9) {
		printf("Wrong arguments.\n");
		return;
	}

	int i;
	for(i = 0; i < 9; i++) {
		if(!(argv[2][i] != 'r' || argv[2][i] != 'w' || argv[2][i] != 'x' || argv[2][i] != '-')) {
			printf("Wrong arguments.\n");
			return;
		}
	}

	CommandArgs* args = malloc(sizeof(CommandArgs));

	args->path = argv[1];
	args->rights = argv[2];

	strcpy(gRights, argv[2]);

	return args;
}

char* concatenateStrings(char* s1, char* s2) {
	int totalSize = strlen(s1) + strlen(s2);

	char* result = malloc((totalSize + 2) * sizeof(char));
	memset(result, '\0', totalSize);

	strncat(result, s1, strlen(s1));
	strncat(result, "/", 1);
	strncat(result, s2, strlen(s2));

	return result;
}

int hasPermissions(mode_t st_mode, char* rights) {
	int result = 0;
	char* checkedRights = malloc(10 * sizeof(char));

	checkedRights[0] = (st_mode & S_IRUSR) ? 'r' : '-';
	checkedRights[1] = (st_mode & S_IWUSR) ? 'w' : '-';
	checkedRights[2] = (st_mode & S_IXUSR) ? 'x' : '-';

	checkedRights[3] = (st_mode & S_IRGRP) ? 'r' : '-';
	checkedRights[4] = (st_mode & S_IWGRP) ? 'w' : '-';
	checkedRights[5] = (st_mode & S_IXGRP) ? 'x' : '-';

	checkedRights[6] = (st_mode & S_IROTH) ? 'r' : '-';
	checkedRights[7] = (st_mode & S_IWOTH) ? 'w' : '-';
	checkedRights[8] = (st_mode & S_IXOTH) ? 'x' : '-';

	checkedRights[9] = '\0';

	if(strcmp(rights, checkedRights) == 0) result = 1;

	free(checkedRights);

	return result;
}

char* parseToPath(char* path, char* filename, int level) {
	char* result;

	if(level == 0) {
		int length = (2 + strlen(filename));

		result = malloc(length * sizeof(char));
		memset(result, '\0', length);

		strncat(result, "/", 2);
		strncat(result, filename, length - 2);

		return result;
	}

	int pathLength = strlen(path);
	int filenameLength = strlen(filename);

	int i;
	for(i = pathLength - 1; i >= 0 && level > 0; i--) {
		if(*(path + i) ==  '/') level--;
	}

	int size = (2 + (pathLength - i) + filenameLength);

	result = malloc(size * sizeof(char));
	memset(result, '\0', size);

	strncat(result, path + i + 1, pathLength - i);
	strncat(result, "/", 1);
	strncat(result, filename, filenameLength);

	return result;
}

void firstVariant(char* path, int level, char* rights) {
	DIR* dir;

	if((dir = opendir(path)) == NULL) {
		printf("Couldn't open the dir.\n");
		return;
	}

	struct dirent* dnt;
	char* filename;
	struct stat* buf;

	while((dnt = readdir(dir)) != NULL) {
		filename = concatenateStrings(path, dnt->d_name);
		buf = malloc(sizeof(struct stat));

		stat(filename, buf);
		if(S_ISREG(buf->st_mode) && hasPermissions(buf->st_mode, rights)) {
			printf("\nFILENAME: %s\nSIZE: %zu bytes\nLAST ACCESS: %s\n", parseToPath(path, dnt->d_name, level), buf->st_size, ctime(&(buf->st_atime)));
		}
		else if(S_ISDIR(buf->st_mode) && !(strcmp(dnt->d_name, ".") == 0 || strcmp(dnt->d_name, "..") == 0)) {
			firstVariant(filename, level + 1, rights);
		}

		free(buf);
		free(filename);
	}


	closedir(dir);
}

int crawler(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
	if(typeflag != FTW_F) {
		return 0;
	}

	if(hasPermissions(sb->st_mode, gRights)) {
		printf("\nFILENAME: %s\nSIZE: %zu bytes\nLAST ACCESS: %s\n", fpath, sb->st_size, ctime(&(sb->st_atime)));
	}

	return 0;
}

void secondVariant(char* path) {
	nftw(path, crawler, 100, FTW_PHYS);
}

int main(int argc, char** argv) {
	CommandArgs* args = getCommandArgs(argc, argv);

	printf("\nUSING OPENDIR / READDIR\n");
	printf("=====================================================================================\n");
	firstVariant(args->path, 0, args->rights);
	printf("\nUSING NFTW LIBRARY\n");
	printf("=====================================================================================\n");
	secondVariant(args->path);

	free(args);

	return 0;
}