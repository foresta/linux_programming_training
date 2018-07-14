#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "%s: wrong arguments\n", argv[0]);
        exit(1);
    }

    char* src = argv[1];
    char* dst = argv[2];
    if (link(src, dst) < 0) {
        perror(argv[1]);
        exit(1);
    }

    exit(0);
}
