#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>

#define R_LEN 1024

static int file;
static pthread_t* threads;

struct record {
    int id;
    char text[R_LEN];
};

struct args {
    int n_threads;
    char* file_name;
    int n_records;
    char* word;
};

void help_message();

void print_args(struct args arg);

struct args parse_args(int argc, char** argv);

void assert(int cond, const char* message);

void clear_record(struct record* r);

void fill_record(char line[R_LEN], struct record* record);

int read_and_fill(struct record* r, int r_number);

int find_word(char* line, char* word);

void* task(void* args);

int main(int argc, char** argv) {
    struct args arg = parse_args(argc, argv);
    print_args(arg);

    file = open(arg.file_name, O_RDONLY);
    assert(file != -1, "Error when opening file");

    threads = malloc(arg.n_threads * sizeof(pthread_t));
    
    int i, res;
    for(i = 0; i < arg.n_threads; i++) {
	res = pthread_create(threads + i, NULL, task,(void*)&arg);
	assert(res == 0, "Error when spawning thread");
    }
    
    

    printf("Cleaning up...\n");
    free(threads);
    free(arg.file_name);
    free(arg.word);
}

void help_message() {
    printf("\nUsage: ./main <n_threads> <file_name> <records_to_read> <word_to_find>\n");
}

struct args parse_args(int argc, char** argv) {
    if (argc < 5) {
	help_message();
	exit(EXIT_FAILURE);
    }

    struct args arg;
    arg.n_threads = (int) strtol(argv[1], NULL, 10);
    arg.n_records = (int) strtol(argv[3], NULL, 10);
    arg.file_name = malloc((strlen(argv[2]) + 1) * sizeof(char));
    arg.word = malloc((strlen(argv[4]) + 1) * sizeof(char));
    strcpy(arg.file_name, argv[2]);
    strcpy(arg.word, argv[4]);
    return arg;
}

void print_args(struct args arg) {
    printf("Number of threads: %d\n", arg.n_threads);
    printf("Number of records to read at a time: %d\n", arg.n_records);
    printf("File to read from: %s\n", arg.file_name);
    printf("Word to find: %s\n", arg.word);
}

void assert(int cond, const char* message) {
    if (!cond) {
	perror(message);
	exit(EXIT_FAILURE);
    }
}

void clear_record(struct record* r) {
    int i;
    for(i = 0; i < R_LEN; i++) {
	r -> text[i] = 0;
    }
    r -> id = 0;
}

void fill_record(char line[R_LEN], struct record* r) {
    int current_read;
    int id = 0;
    int i = 0;
    while(1) {
	if(line[i] == '.') break;
	current_read = (int)line[i] - (int)('0');
	id = 10 * id + current_read;
	i += 1;
    }
    int id_len = i + 1;
    for(i = id_len; i < R_LEN; i++) {
	r -> text[i - id_len] = line[i];
    }
    r -> id = id;
}

int read_and_fill(struct record* r, int r_number) {
    char buffer[R_LEN];
    int n_read = pread(file, buffer, R_LEN, (r_number - 1) * R_LEN);
    clear_record(r);
    fill_record(buffer, r);
    return n_read;
}

int find_word(char* line, char* word) {
    int line_len = strlen(line);
    int word_len = strlen(word);
    int i, j;
    for(i = 0; i <= line_len - word_len; i++) {
	if(line[i] == word[0]) {
	    for(j = 0; j < word_len; j++) {
		if(line[i + j] != word[j]) break;
		if(j == word_len - 1) return i;
	    }
	}
    }
    return -1;
}

void* task(void* arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    struct args a = *((struct args*)arg);
    struct record r[a.n_records];
    pthread_t self = pthread_self();

    int word_pos;
    int current_record = 1;
    int finished = 0;

    int i;
    while(1) {
	for(i = 0; i < a.n_records; i++) {
	    if(read_and_fill(&r[i], current_record) == 0) finished = 1;
	    current_record += 1;
	}
	if(finished) break;
	for(i = 0; i < a.n_records; i++) {
	    word_pos = find_word(r[i].text, a.word);
	    if(word_pos != -1) {
		printf("TID: %u, found word at id: %d, position: %d\n",
		       (unsigned int)self, r[i].id, word_pos);
		int j;
		for(j = 0; j < a.n_threads; j++) {
		    if(pthread_equal(self, threads[j]) == 0) pthread_cancel(threads[j]);
		}
		pthread_exit(0);
	    }
	}
    }
    pthread_exit(0);
}
