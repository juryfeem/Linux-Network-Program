#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"

//文件存储路径
#define FILE_DIR "../documents"

// main函数传参 argv[1] -> port,否则默认10086,然后启动epoll
int main(int argc, char *argv[])
{
    int port = 10086;
    // 确认端口
    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    printf("server start on port %d\n", port);

    // 改变进程工作目录
    int ret = chdir(FILE_DIR);
    if (ret != 0)
    {
        perror("chdir error");
        exit(1);
    }

    // 启动epoll
    epoll_run(port);

    return 0;
}