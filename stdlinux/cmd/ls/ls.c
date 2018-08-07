#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

static void do_ls(char* path);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        do_ls(".");
        exit(0);
    }

    int i;
    for (i = 1; i < argc; i++) {
        do_ls(argv[1]); 
    }
    exit(0);
}


static void do_ls(char* path)
{
    DIR* d = opendir(path); 
    if (!d) {
        perror(path);
        exit(1);
    }

    struct dirent *ent;
    while ( (ent = readdir(d)) ) {
        printf("%s\t", ent->d_name);
    }

    closedir(d);
}
