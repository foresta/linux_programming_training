#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define _GNU_SOURCE
#include <getopt.h>

#define BUFFER_SIZE 2048

static void do_cat(FILE* input, int escapeFlag);


int main (int argc, char *argv[])
{
    int opt;
    int escapeFlag = 0;
    while ( (opt = getopt(argc, argv, "e")) != -1) {
        switch (opt) {
            case 'e':
               escapeFlag = 1;
               break;

            case '?':
              fprintf(stderr, "Usage: %s [-e] [file ...]\n", argv[0]);
              exit(1);
              break;
        }
    }

    printf("argc is %d\n", argc);
    int j;
    for (j = 0; j < argc; j++) { printf("argv[%d] is %s\n", j, argv[j]); }
    printf("escapeFlag is %d\n", escapeFlag);

    if (escapeFlag == 1)
    {
        if (argc == 2) {
            do_cat(stdin, escapeFlag);
        }
        else {
            int i;
            for (i = 2; i < argc; i++) {
                char* path =  argv[i];
                FILE* f = fopen(path, "r");
                if (!f) {
                    perror(path);
                    exit(1);
                }
                do_cat(f, escapeFlag);
                fclose(f);
            }
        }
    }
    else {
        if (argc == 1) {
            do_cat(stdin, escapeFlag);
        }
        else {
            int i;
            for (i = 1; i < argc; i++) {
                char* path =  argv[i];
                FILE* f = fopen(path, "r");
                if (!f) {
                    perror(path);
                    exit(1);
                }
                do_cat(f, escapeFlag);
                fclose(f);
            }
        }
    }

    exit(0);
}

static void do_cat(FILE* f, int escapeFlag)
{
    int c;
    while((c = fgetc(f)) != EOF) {
        char character = (char) c;
        if (escapeFlag == 1) {

            if (character == '\t') {
                if (fputs("\\t", stdout) == EOF) exit(1);
            }
            else if (character == '\n') {
                if (fputs("$\n", stdout) == EOF) exit(1);
            }
            else {
                if (putchar(c) < 0) exit(1);
            }
        } else {
            if (putchar(c) < 0) exit(1);
        }
    }
}
