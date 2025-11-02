#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <sys/stat.h>

void print_file_info(const char *path, struct stat *st, Options *opts);
void human_readable(long size, char *buf, size_t buf_size);


#endif
