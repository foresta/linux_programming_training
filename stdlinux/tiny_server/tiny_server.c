#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

static void service(FILE* in, FILE* out, char* docroot);

// signal handling
static void install_signal_handlers(void);
static void trap_signal(int sig, sighandler_t handler);
static void signal_exit(int sig);

static void* xmalloc(size_t size);
static void log_exit(char* fmt, ...);


int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <docroot>\n", argv[0]);
        exit(1);
    }

    // TODO::docrootがディレクトリかどうかをチェックする

    // シグナルハンドラを設置
    install_signal_handlers();

    // サーバー開始
    service(stdin, stdout, argv[1]);

    exit(0);
}

static void service(FILE* in, FILE* out, char* docroot)
{

}

// ---------------------------
// signal
// ---------------------------
static void install_signal_handlers(void)
{
    trap_signal(SIGPIPE, signal_exit);
}

static void trap_signal(int sig, sighandler_t handler)
{
    struct sigaction act;
   
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flag = SA_RESTART;
    if (sigaction(sig, &act, NULL) < 0)
        log_exit("sigaction() failed: %s", strerror(errno))
}

static void signal_exit(int sig)
{
    log_exit("exit by signal %d", sig);
}

// ---------------------------
// utility
// ---------------------------

// Memory Allocation
static void* xmalloc(size_t size)
{
    void* p = malloc(size);
    if (!p) log_exit("failed to allocate memory");

    return p;
}


// Error Handling And Logging
static void log_exit(char* fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);

    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);

    exit(1);
}
