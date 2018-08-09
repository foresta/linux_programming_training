#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

// constants



// --------------

struct HTTPHeaderField {
    char* name;
    char* value;
    struct HTTPHeaderField* next;
}

struct HTTPRequest {
    int protocol_minor_version;
    char* method;
    char* path;
    struct HTTPHeaderField* header;
    char* body;
    long length;
}

static void service(FILE* in, FILE* out, char* docroot);
static struct HTTPRequest* read_request(FILE* in);
static void read_request_line(struct HTTPRequest* req, FILE* in);
static struct HTTPHeaderField* read_header_field(FILE* in);
static long content_length(struct HTTPRequest* req);
static void free_request(struct HTTPRequest* req);

// signal handling
static void install_signal_handlers(void);
static void trap_signal(int sig, sighandler_t handler);
static void signal_exit(int sig);

// utilities
static void* xmalloc(size_t size);
static void log_exit(char* fmt, ...);
static int is_directory(char* path);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <docroot>\n", argv[0]);
        exit(1);
    }

    char* docroot = argv[1];
    if (!is_directory(docroot))
    {
        fprintf(stderr, "docroot %s is not directory", docroot);
        exit(1);
    }

    // シグナルハンドラを設置
    install_signal_handlers();

    // サーバー開始
    service(stdin, stdout, docroot);

    exit(0);
}

static void service(FILE* in, FILE* out, char* docroot)
{
    struct HTTPRequest* req;
    req = read_request(in);
    respond_to(req, out, docroot);
    free_request(req);
}

static struct HTTPRequest* read_request(FILE* in)
{
    struct HTTPRequest* req = xmalloc(sizeof(struct HTTPRequest));
    read_request_line(req, in);
    req->header = NULL;

    while ( h = read_header_field(in) ) {
        h->next = req->header;
        req->header = h;
    }
    req->length = content_length(req);
    if (req->length != 0) {
        if (rq->length > MAX_REQUEST_BODY_LENGTH)
            log_exit("request body too long");
        
        req->body = xmalloc(req->length);
        if (fread(req->body, req->length, 1, in) < 1) 
            log_exit("failed to read request body");
    } else {
        req->body = NULL;
    }

    return req;
}

static void read_request_line(struct HTTPRequest* req, FILE* in)
{

}

static struct HTTPHeaderField* read_header_field(FILE* in)
{

}

static long content_length(struct HTTPRequest* req)
{

}

static void free_request(struct HTTPRequest* req)
{
    struct HTTPHeaderField *h;
    struct HTTPHeaderField* head = req->header;
    while(head)
    {
        h = head;
        head = head->next;

        free(h->name);
        free(h->value);
        free(h)
    }
    free(req->method);
    free(req->path);
    free(req->body);
    free(req);
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

static int is_directory(char* path)
{
    struct stat st;
    if (lstat(path, &st) < 0) {
        perror(path);
        exit(1);
    }

    return S_ISDIR(st.st_mode);
}
