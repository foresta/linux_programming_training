#include <stdio.h>
#include <stdlib.h>

static void do_cat(FILE* f);

int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        fprintf(stderr, "%s: file name not give\n", argv[0]);
        exit(1);
    }

    int i;
    for (i = 1; i < argc; i++) {
        char* path =  argv[i];
        FILE* f = fopen(path, "r");
        if (!f) {
            perror(path);
            exit(1);
        }
        do_cat(f);
        fclose(f);
    }

    exit(0);
}

static void do_cat(FILE* f)
{
    int c;
    while((c = fgetc(f)) != EOF) {
        char character = (char) c;
        if (character == '\t') {
            if (fputs("\\t", stdout) == EOF) exit(1);
        }
        else if (character == '\n') {
            if (fputs("$\n", stdout) == EOF) exit(1);
        }
        else {
            if (putchar(c) < 0) exit(1);
        }
    }
}
