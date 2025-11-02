#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct {
    int show_all;      // -a
    int long_list;     // -l
    int recursive;     // -R
    int reverse;       // -r
    int sort_time;     // -t
    int sort_size;     // -S
    int human_read;    // -h
    int show_inode;    // -i
    int show_blocks;   // -s
    int classify;      // -F
} Options;


void parse_options(int argc, char *argv[], Options *opts, char ***paths, int *path_count);

#endif