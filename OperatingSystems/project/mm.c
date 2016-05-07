#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ftw.h>
#define _GNU_SOURCE

void print_usage() {
    fprintf(stdout,
        "\n\n  Usage information for mm code tracker system.\n"
        "  -h   --help                    Print usage information.\n"                          // done
        "  -i   --init                    Initialize repository in the working directory.\n"   // done
        "  -s   --status                  Show current status (state) of repository.\n"
        "  -a   --add [files \\ all]      Track file.\n"
        "  -r   --remove [files \\ all]   Stop tracking file.\n"
        "  -c   --commit [message]        Commit changes.\n"
        "  -p   --previous                Show commits' history.\n"
        "  -u   --upload                  Push current repository state into remote server.\n"
        "  -d   --download                Download repository state from remote server.\n\n\n"
     );

     exit(1);
}

int exists(DIR *dir) {
    struct dirent *dnt;

    while ( (dnt = readdir(dir)) != NULL ) {
        if ( strcmp(dnt->d_name, ".mm") == 0 ) {
            return 1;
        }
    }

    return 0;
}

void init_repo() {
    DIR *dir;
    FILE *fp;

    if ( (dir = opendir(".")) == NULL ) {
        fprintf(stderr, "Couldn't open working directory.\n");
        exit(-1);
    }

    if ( exists(dir) ) {
        fprintf(stdout, "Repository already exists.\n");
        exit(1);
    }

    mkdir(".mm", 0700);
    
    if ( (fp = fopen("./.mm/tracking.txt", "w+")) == NULL ) {
        fprintf(stderr, "Couldn't create trackfile.\n");
        exit(-1);
    }
    
    fclose(fp);
    closedir(dir);

    exit(1);
}

void add_all(DIR *dir) {
    
}

void add(int argc, char **argv) {
    DIR *dir;

    if ( (dir = opendir(".")) == NULL ) {
        fprintf(stderr, "Couldn't open working directory.\n");
        exit(-1);
    }

    if ( !exists(dir) ) {
        fprintf(stderr, "There is no repository initialized in the working directory.\n");
        exit(-1);
    }

    if ( strcmp("all", argv[2]) == 0 ) {
        add_all(dir);
    } else {
        add_list(dir, argv + 2, argc - 2);
    }

    closedir(dir);
}

int main(int argc, char **argv) {

    const char* const short_options = "hisa:r:cpud";

    const struct option long_options[] = {
        { "help",       0,  NULL,   'h' },
        { "init",       0,  NULL,   'i' },
        { "status",     0,  NULL,   's' },
        { "add",        1,  NULL,   'a' },
        { "remove",     1,  NULL,   'r' },
        { "commit",     0,  NULL,   'c' },
        { "previous",   0,  NULL,   'p' },
        { "upload",     0,  NULL,   'u' },
        { "download",   0,  NULL,   'd' }
    };

    int current_option = getopt_long(argc, argv,
            short_options, long_options, NULL);

    switch (current_option) {
        case '?': case -1: case 'h':
            print_usage();
        case 'i':
            init_repo();
        case 'a':
            add(argc, argv);
    }

}
