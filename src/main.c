#include "ls.h"
#include <locale.h>

int main(int argc, char *argv[]) {
    setlocale(LC_COLLATE, "");
    Options opts = {0};
    char **paths = NULL;
    int path_count = 0;

    parse_options(argc, argv, &opts, &paths, &path_count);

    if (path_count == 0) {
        display_directory(".", &opts);
    } else {
        for (int i = 0; i < path_count; i++) {
            display_directory(paths[i], &opts);
        }
    }

    free(paths);
    return 0;
}
