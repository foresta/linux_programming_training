#include <stdio.h>
#include <stdlib.h>

static int do_line_count(FILE* f);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "%s: file name not given\n", argv[0]);
        exit(1);
    }

    int i;
    for (i = 1; i < argc; i++) {
        char* path = argv[i];
        FILE* f = fopen(path, "r");
        if (!f) {
            perror(path);
            exit(1);
        }
        printf("%d\n", do_line_count(f));
    }
    
    exit(0);
}

static int do_line_count(FILE* f)
{
    int c;
    int prev = '\n';

    int lineCount = 0;
    while((c = fgetc(f)) != EOF) {
       if (c== '\n') lineCount++;

       prev = c;
    }

    if (prev != '\n') lineCount++;
  
    return lineCount;
}
