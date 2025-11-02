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

    time_t display_time = st->st_mtime;
    if (opts->sort_ctime) display_time = st->st_ctime;
    else if (opts->sort_atime) display_time = st->st_atime;

    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&display_time));

    char sizebuf[32];
    if (opts->human_read) {
        human_readable(st->st_size, sizebuf, sizeof(sizebuf));
    } else if (opts->kilobyte_unit) {
        snprintf(sizebuf, sizeof(sizebuf), "%ldK", st->st_size / 1024);
    } else {
        snprintf(sizebuf, sizeof(sizebuf), "%ld", st->st_size);
    }

    if (opts->numeric_id) {
        printf("%s %3ld %5d %5d %8s %s %s\n",
               perms,
               (long)st->st_nlink,
               st->st_uid,
               st->st_gid,
               sizebuf,
               timebuf,
               path);
    } else {
        struct passwd *pw = getpwuid(st->st_uid);
        struct group *gr = getgrgid(st->st_gid);
        printf("%s %3ld %-8s %-8s %8s %s %s\n",
               perms,
               (long)st->st_nlink,
               pw ? pw->pw_name : "unknown",
               gr ? gr->gr_name : "unknown",
               sizebuf,
               timebuf,
               path);
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
