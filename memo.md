Linux

3つの抽象概念
* ファイル
* プロセス
* ストリーム

## ストリーム操作のシステムコール
open
close
read
write

## システムコールのラッパー(ライブラリ)

fgetc
fputc

getchar stdin
putchar stdout

ungetc
fgets
    (getsは絶対に使わない)
fputs
puts  末尾に\nを不可

printf
fprintf

scanfは使わない(バッファオーバーフローの危険性)

## 固定長入出力API
fread(), fwrite()

## ファイルオフセット
fseek, feeko
fseekoを使ってればOK

ファイルオフセット値取得
ftell(FILE\*)
ftello() <- こっち使っとく

先頭に戻す
rewind(FILE\*)

## FILE型

```
int fileno(FILE* stream)
FILE* fopen(const char* path, const char* mode);
FILE* fdopen(int fd, const char* mode);
```

fopenはパーミッションの指定などができない
```
int fd = open(...)
// set permission
FILE* file = fdopen(fd, "r");
```

ioctl(), fcntl()に対応する操作もない

## バッファリング

バッファをフラッシュ
```
#include <stdio.h>
int fflush(FILE* stream);
```

setvbuf

## EOFとエラー

```
int feof(FILE* stream)
int ferror(FILE* stream) // fread()みたいなエラーとEOFが区別できないAPI用
void clearerr(FILE* stream) // streamのerror, EOFをクリア(tail -fなど)
```


## オプション解析
getopt, getopt_long はGNU libc特有なので、LinuxOS以外では使えないと思っておいた方が良い

## gdb

```
$ gdb ./app # gdb起動
(gdb) run [options] # app実行
(gdb) backtrace
(gdb) frame N  # フレームに移動(Nはbacktraceで見れる)
(gdb) list # ソースコード表示
(gdb) print EXPR # 式を評価した値を表示
(gdb) quit # gdb終了
```

breakpointのやり方しらべとく



