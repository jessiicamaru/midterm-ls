#include "ls.h"
#include <sys/types.h>

typedef struct {
    char name[256];
    struct stat st;
} FileEntry;

/* ---------------- SORTING FUNCTIONS ---------------- */

int compare_name(const void *a, const void *b) {
    return strcmp(((FileEntry*)a)->name, ((FileEntry*)b)->name);
}

int compare_time(const void *a, const void *b) {
    time_t ta = ((FileEntry*)a)->st.st_mtime;
    time_t tb = ((FileEntry*)b)->st.st_mtime;
    if (tb > ta) return 1;
    if (tb < ta) return -1;
    return 0;
}

int compare_size(const void *a, const void *b) {
    off_t sa = ((FileEntry*)a)->st.st_size;
    off_t sb = ((FileEntry*)b)->st.st_size;
    if (sb > sa) return 1;
    if (sb < sa) return -1;
    return 0;
}

/* ---------------- MAIN DIRECTORY DISPLAY ---------------- */

void display_directory(const char *path, Options *opts) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror(path);
        return;
    }

    struct dirent *entry;
    FileEntry *files = NULL;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!opts->show_all && entry->d_name[0] == '.')
            continue;

        files = realloc(files, sizeof(FileEntry) * (count + 1));
        if (!files) {
            fprintf(stderr, "Memory allocation error\n");
            closedir(dir);
            return;
        }

        snprintf(files[count].name, sizeof(files[count].name), "%s", entry->d_name);

        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        if (stat(fullpath, &files[count].st) != 0) {
            perror(fullpath);
            continue;
        }

        count++;
    }
    closedir(dir);

    // Sắp xếp theo flag
    if (opts->sort_time)
        qsort(files, count, sizeof(FileEntry), compare_time);
    else if (opts->sort_size)
        qsort(files, count, sizeof(FileEntry), compare_size);
    else
        qsort(files, count, sizeof(FileEntry), compare_name);

    // Đảo ngược nếu có flag -r
    if (opts->reverse) {
        for (int i = 0; i < count / 2; i++) {
            FileEntry tmp = files[i];
            files[i] = files[count - 1 - i];
            files[count - 1 - i] = tmp;
        }
    }

    printf("%s:\n", path);

    // In danh sách file
    for (int i = 0; i < count; i++) {
        struct stat *st = &files[i].st;

        if (opts->show_inode)
            printf("%8lu ", st->st_ino);

        if (opts->show_blocks)
            printf("%4ld ", st->st_blocks / 2); // 512 bytes mỗi block

        if (opts->long_list)
            print_file_info(files[i].name, st, opts);
        else {
            printf("%s", files[i].name);

            // -F: thêm ký hiệu phân loại
            if (opts->classify) {
                if (S_ISDIR(st->st_mode)) printf("/");
                else if (S_ISLNK(st->st_mode)) printf("@");
                else if (st->st_mode & S_IXUSR) printf("*");
            }
            printf("  ");
        }
    }

    if (!opts->long_list)
        printf("\n");

    // -R: hiển thị đệ quy
    if (opts->recursive) {
        for (int i = 0; i < count; i++) {
            if (S_ISDIR(files[i].st.st_mode)
                && strcmp(files[i].name, ".") != 0
                && strcmp(files[i].name, "..") != 0) {

                char newpath[1024];
                snprintf(newpath, sizeof(newpath), "%s/%s", path, files[i].name);
                printf("\n");
                display_directory(newpath, opts);
            }
        }
    }

    free(files);
}

/* ---------------- SINGLE FILE DISPLAY ---------------- */

void display_file(const char *path, Options *opts) {
    struct stat st;
    if (stat(path, &st) == 0) {
        if (opts->show_inode)
            printf("%8lu ", st.st_ino);

        if (opts->show_blocks)
            printf("%4ld ", st.st_blocks / 2);

        if (opts->long_list)
            print_file_info(path, &st, opts);
        else {
            printf("%s", path);
            if (opts->classify) {
                if (S_ISDIR(st.st_mode)) printf("/");
                else if (S_ISLNK(st.st_mode)) printf("@");
                else if (st.st_mode & S_IXUSR) printf("*");
            }
            printf("\n");
        }
    } else {
        perror(path);
    }
}
