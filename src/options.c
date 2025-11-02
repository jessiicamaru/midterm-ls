#include "ls.h"

void parse_options(int argc, char *argv[], Options *opts, char ***paths, int *path_count) {
    memset(opts, 0, sizeof(Options));
    *path_count = 0;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (char *c = argv[i] + 1; *c; c++) {
                switch (*c) {
                    case 'a': opts->show_all = 1; break;
                    case 'A': opts->almost_all = 1; break;
                    case 'R':
                        opts->recursive = 1;
                        opts->list_dir_as_file = 0; 
                        break;
                    case 'd':
                        opts->list_dir_as_file = 1;
                        opts->recursive = 0; 
                        break;
                    case 'r': opts->reverse = 1; break;
                    case 't': opts->sort_time = 1; break;
                    case 'S': opts->sort_size = 1; break;
                    case 'h': opts->human_read = 1; opts->kilobyte_unit = 0; break;
                    case 'i': opts->show_inode = 1; break;
                    case 's': opts->show_blocks = 1; break;
                    case 'F': opts->classify = 1; break;
                    case 'l': 
                        opts->long_list = 1; 
                        opts->numeric_id = 0; 
                        break;
                    case 'n': 
                        opts->long_list = 1; 
                        opts->numeric_id = 1; 
                        break;
                    case 'c': opts->sort_ctime = 1; opts->sort_atime = 0; break;
                    case 'u': opts->sort_atime = 1; opts->sort_ctime = 0; break;
                    case 'f': opts->unsorted = 1; break;
                    case 'k': opts->kilobyte_unit = 1; opts->human_read = 0; break;
                    case 'q': opts->quote_nonprint = 1; opts->raw_nonprint = 0; break;
                    case 'w': opts->raw_nonprint = 1; opts->quote_nonprint = 0; break;
                    default:
                        fprintf(stderr, "Unknown option: -%c\n", *c);
                        exit(EXIT_FAILURE);
                }
            }
        } else {
            (*path_count)++;
        }
    }

    *paths = malloc(sizeof(char*) * (*path_count));
    int idx = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            (*paths)[idx++] = argv[i];
        }
    }
}

