#include "ls.h"

void parse_options(int argc, char *argv[], Options *opts, char ***paths, int *path_count) {
    *path_count = 0;
    *paths = NULL;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'a': opts->show_all = 1; break;
                    case 'l': opts->long_list = 1; break;
                    case 'R': opts->recursive = 1; break;
                    case 'r': opts->reverse = 1; break;
                    case 't': opts->sort_time = 1; break;
                    case 'S': opts->sort_size = 1; break;
                    case 'h': opts->human_read = 1; break;
                    case 'i': opts->show_inode = 1; break;
                    case 's': opts->show_blocks = 1; break;
                    case 'F': opts->classify = 1; break;
                    default:
                        fprintf(stderr, "ls: invalid option -- '%c'\n", argv[i][j]);
                        exit(EXIT_FAILURE);
                }
            }
        } else {
            *paths = realloc(*paths, sizeof(char *) * (*path_count + 1));
            (*paths)[*path_count] = argv[i];
            (*path_count)++;
        }
    }
}
