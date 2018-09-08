// remove_dir_rec.cpp.cpp
// May require C11 or later
// BUILD: clang++ -std=c++11 fts_tree.cpp -o tmpl.out && tmpl.out
// BUILD:     g++ -std=c++11 fts_tree.cpp -o tmpl.out && tmpl.out

// #include <dirent.h>
#include <ftw.h>
#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                 // rmdir & unlink
#include <fts.h>
#include <string.h>

int main(int argc, char **argv)
{
    char * const dot[] = {(char *const)".", 0};
    char * const *paths = argc > 1 ? argv + 1 : dot;

    FTS *tree = fts_open(paths, FTS_NOCHDIR, 0);
    if (!tree) {
        perror("fts_open");
        return 1;
    }

    FTSENT *node;
    while ((node = fts_read(tree))) {
        if (node->fts_level > 0 && node->fts_name[0] == '.')
            fts_set(tree, node, FTS_SKIP);
        else if (node->fts_info & FTS_F) {
            printf("got file named %s at depth %d, "
                "accessible via %s from the current directory "
                "or via %s from the original starting directory\n",
                node->fts_name, node->fts_level,
                node->fts_accpath, node->fts_path);
            /* if fts_open is not given FTS_NOCHDIR,
             * fts may change the program's current working directory */
        }
    }
    if (errno) {
        perror("fts_read");
        return 1;
    }

    if (fts_close(tree)) {
        perror("fts_close");
        return 1;
    }

    return 0;
}
