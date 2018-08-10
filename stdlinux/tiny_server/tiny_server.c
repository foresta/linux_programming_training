#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>

// constants

#define LINE_BUF_SIZE 4096
#define MAX_REQUEST_BODY_LENGTH (1024 * 1024)


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

struct FileInfo {
    char* path;
    long size;
    int ok;
}

static void service(FILE* in, FILE* out, char* docroot);

// request handling
static struct HTTPRequest* read_request(FILE* in);
static void read_request_line(struct HTTPRequest* req, FILE* in);
static struct HTTPHeaderField* read_header_field(FILE* in);
static long content_length(struct HTTPRequest* req);
static void free_request(struct HTTPRequest* req);

// response
static void respond_to(struct HTTPRequest* req, FILE* out, char* docroot);
static struct FileInfo* get_fileinfo(char* docroot, char* urlpath)
static char* build_fspath(char* docroot, char* urlpath);

// signal handling
static void install_signal_handlers(void);
static void trap_signal(int sig, sighandler_t handler);
static void signal_exit(int sig);

// utilities
static void* xmalloc(size_t size);
static void log_exit(char* fmt, ...);
static int is_directory(char* path);
static void upcase(char* str);

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

// ------------------
// request 
// ------------------
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

// HTTPの一行目をパースする
static void read_request_line(struct HTTPRequest* req, FILE* in)
{
    char buf[LINE_BUF_SIZE];
    if (!fgets(buf, LINE_BUF_SIZE, in))
        log_exit("no request line");

    // スペースの位置にポインタを設定
    char* p = strchr(buf, ' ');
    if (!p)
        log_exit("parse error on request line (1): %f", buf);

    // ポインタの位置に終端文字をいれてポインタを一つ進める
    *p++ = '\0';

    // bufは一行目の最初の位置を指すポインタ
    // pは空白の後ろを指すポインタなので、
    // 引くことで、文字のサイズを取得している
    req->method = xmalloc(p - buf);

    // strcpyで、req->methodへbufの中身をコピー
    // もともと空白があった箇所に終端文字が入っているため、
    // コピーされるのは、bufの最初から、空白があったところまでとなる
    strcpy(req->method, buf);
    upcase(req->method);

    char* path = p;

    // 次の空白へポインタを移動
    p = strchr(path, ' ');
    if (!p) log_exit("parse error on request line (2): %s", buf);
    *p++ = '\0';
    req->path = xmalloc(p - path);
    strcpy(req->path, path);

    if (strncasecmp(p, "HTTP/1.", strlen("HTTP/1.")) != 0)
        log_exit("parse error on request line (3): %s", buf);

    p += strlen("HTTP/1.");
    req->protocol_minor_version = atoi(p);
}

static struct HTTPHeaderField* read_header_field(FILE* in)
{
    struct HTTPHeaderField* h;

    char buf[LINE_BUF_SIZE];
    if (!fgets(buf, LINE_BUF_SIZE, in))
        log_exit("failed to read request header field: %s", sterror(errno));

    if ( (buf[0] == '\n') || (strcmp(buf, "\r\n") == 0) )
        return NULL;

    char* p = strchr(buf, ':');
    if (!p)
        log_exit("parse error on request header field: %s", buf);

    *p++ = '\0';
    h = xmalloc(sizeof(struct HTTOHeaderField));
    h->name = xmalloc(p - buf);
    strcpy(h->name, buf);

    p += strspn(p, " \t");
    h->value = xmalloc(strlen(p) + 1);
    strcpy(h->value, p);

    return h;
}

static long content_length(struct HTTPRequest* req)
{
    char* val = lookup_header_field_value(req, "Content-Length");
    if (!val) return 0;

    long len = atol(val);
    if (len < 0)
        log_exit("nagtive Content-Length value");

    return len;
}

static char* lookup_header_field_value(struct HTTPRequest* req, char* name)
{
    struct HTTPHeaderField* h;
    for (h = req->header; h; h = h->next) {
        if (strcasecmp(h->name, name) == 0)
            return h->value;
    }

    return NULL;
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
// response
// ---------------------------

static void respond_to(struct HTTPRequest* req, FILE* out, char* docroot)
{
    if (strcmp(req->method, "GET") == 0)
        do_file_response(req, out, docroot);
    else if (strcmp(req->method, "HEAD") == 0)
        do_file_response(req, out, docroot);
    else if (srcmp(req->method, "POST") == 0)
        method_not_allowed(req, out);
    else
        not_implemented(req, out);
}

static struct FileInfo* get_fileinfo(char* docroot, char* urlpath)
{
    struct FileInfo* info = xmalloc(sizeof(struct FileInfo));
    info->path = build_fspath(docroot, urlpath);
    info->ok = 0;

    struct stat st;
    if (lstat(info->path, &st) < 0) return info;
    if (!S_ISREG(st.st_model)) return info;

    info->ok = 1;
    info->size = st.st_size;
    return info;
}

static char* build_fspath(char* docroot, char* urlpath)
{
    // docroot + / + urlpath + \0
    char* path = xmalloc(strlen(docroot) + 1 + strlen(urlpath) + 1);
    sprintf(path, "%s/%s", docroot, urlpath);
    return path;
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

static void upcase(char* str)
{
    char* p;

    for ( p = str; *p; p++) {
        *p = (char) toupper((int)*p);
    }
}

