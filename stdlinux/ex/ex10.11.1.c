#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

static void list(char* path);
static int is_directory(char* path);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "%s: path not given\n", argv[0]);
        exit(1);
    }

    int i;
    for (i = 1; i < argc; i++) {
        list(argv[i]);
    }


    exit(0);
}


static void list(char* path) 
{
    printf("%s\n", path);
    if (is_directory(path))
    {
        printf("DIRECTORY!!\n");

        DIR* d = opendir(path);
        if (!d) {
            perror(path);
            exit(1);
        }

        char* entries[2048];  
        struct dirent * ent;
        int i = 0;
        while ( (ent = readdir(d)) ) {

            printf("[DEBUG] name: %s\n", ent->d_name);

            if (strcmp(ent->d_name, ".") != 0) continue;
            if (strcmp(ent->d_name, "..") != 0) continue;
            
            entries[i++] = ent->d_name;
        }

        closedir(d);

        int entry_size = i;
        printf("[DEBUG] entry_size is %d\n", entry_size);
        for (i = 0; i < entry_size; i++) {
            list(entries[i]);
        }
    }
}

static int is_directory(char* path)
{
    struct stat st;
    if (lstat(path, &st) < 0) {
        perror(path);
        exit(1);
    }

    return S_ISDIR(st.st_mode);
}
