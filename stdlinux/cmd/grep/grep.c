#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_grep(regex_t* pat, FILE* f, int should_invert_match);
static void show_args(int argc, char* argv[]);
static void usage_exit(int err, char* name);

static struct option longopts[] = {
    { "ignore-case" , no_argument, NULL, 'i' },
    { "invert-match", no_argument, NULL, 'v' },
    { "help"        , no_argument, NULL, 'h' },
    { 0, 0, 0, 0 }
};

int main(int argc, char* argv[]) 
{
    char* app_name = argv[0];

    show_args(argc, argv);

    int should_invert_match = 0;
    int regex_compile_flags = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
    
    int opt;
    while ( (opt = getopt_long(argc, argv, "ihv", longopts, NULL)) != -1) {
        switch (opt) {
            case 'i':
                regex_compile_flags |= REG_ICASE;
                break;

            case 'v':
                should_invert_match = 1;
                break;

            case 'h':
                usage_exit(0, app_name);
                break;

            case '?':
                usage_exit(1, app_name);
                break;
        }
    }

    argc -= optind;
    if (argc < 1) {
        fprintf(stderr, "%s: pattern not given\n", argv[0]);
        usage_exit(1, app_name);
    }
    argv += optind;
    
    regex_t pattern;
    int err = regcomp(&pattern, argv[0], regex_compile_flags);
    if (err != 0) {
        char buf[1024];

        regerror(err, &pattern, buf, sizeof buf);
        puts(buf);

        usage_exit(1, app_name);
    }

    if (argc == 1) {
        // read from stdin
        do_grep(&pattern, stdin, should_invert_match);
    } else {
        int i;
        for (i = 1; i < argc; i++) {
            FILE* f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }

            do_grep(&pattern, f, should_invert_match);
            fclose(f);
        }
    }

    regfree(&pattern);
    exit(0);
}

static void do_grep(regex_t* pat, FILE* f, int should_invert_match)
{
    char buf[4096];

    while(fgets(buf, sizeof buf, f)) {

        int regex_result = regexec(pat, buf, 0, NULL, 0);
        int hit = (should_invert_match && regex_result != 0) ||
                  (!should_invert_match && regex_result == 0);

        if (hit) {
            fputs(buf, stdout);
        }
   }

}

static void show_args(int argc, char* argv[])
{
    printf("[DEBUG] argc is %d\n", argc);
    printf("[DEBUG] argv \n");
    int i;
    for (i = 0; i < argc; i++) {
        printf("[DEBUG] argv[%d]: %s \n", i, argv[i]);
    }
    printf("\n\n");
}


static void usage_exit(int err, char* name)
{
    FILE* output;
    if (err == 1) {
        output = stderr;
    }
    else {
        output = stdout;
    }
    fprintf(output, "Usage: %s [-i IGNORE-CASE] [PATTERN] [FILE ...]\n", name);
    exit(err);
}
