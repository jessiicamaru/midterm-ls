#include "ls.h"

void print_file_info(const char *path, struct stat *st, Options *opts) {
    char perms[11];
    snprintf(perms, sizeof(perms), "----------");

    if (S_ISDIR(st->st_mode)) perms[0] = 'd';
    else if (S_ISLNK(st->st_mode)) perms[0] = 'l';

    if (st->st_mode & S_IRUSR) perms[1] = 'r';
    if (st->st_mode & S_IWUSR) perms[2] = 'w';
    if (st->st_mode & S_IXUSR) perms[3] = 'x';
    if (st->st_mode & S_IRGRP) perms[4] = 'r';
    if (st->st_mode & S_IWGRP) perms[5] = 'w';
    if (st->st_mode & S_IXGRP) perms[6] = 'x';
    if (st->st_mode & S_IROTH) perms[7] = 'r';
    if (st->st_mode & S_IWOTH) perms[8] = 'w';
    if (st->st_mode & S_IXOTH) perms[9] = 'x';

    struct passwd *pw = getpwuid(st->st_uid);
    struct group *gr = getgrgid(st->st_gid);
    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&st->st_mtime));

    printf("%s %3ld %-8s %-8s %8ld %s %s\n",
           perms,
           (long)st->st_nlink,
           pw ? pw->pw_name : "unknown",
           gr ? gr->gr_name : "unknown",
           (long)st->st_size,
           timebuf,
           path);

    if (opts->classify) {
        if (S_ISDIR(st->st_mode)) 
            printf("/");
        else if (S_ISLNK(st->st_mode)) 
            printf("@");
        else if (st->st_mode & S_IXUSR) 
            printf("*");
    }
}

void human_readable(long size, char *buf, size_t buf_size) {
    const char *units[] = {"B", "K", "M", "G", "T"};
    int i = 0;
    double s = (double)size;
    while (s >= 1024 && i < 4) {
        s /= 1024;
        i++;
    }
    snprintf(buf, buf_size, "%.1f%s", s, units[i]);
}
