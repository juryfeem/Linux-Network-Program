#include"wrap.h"

//服务器端口
#define SERV_PORT 10086

//错误输出函数
void sys_err(const char *str){
    perror(str);
    exit(1);
}

int main(){
    int cfd = 0;
    int counter = 10;
    char buf[BUFSIZ];
    struct sockaddr_in serv_addr;

    //创建socket
    cfd = Socket(AF_INET, SOCK_STREAM, 0);

    //连接服务器
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    int ret = Connect(cfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //写数据
    while(--counter){
        write(cfd, "hello\n", 6);
        ret = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
        sleep(1);
    }

    //关闭socket
    close(cfd);

    return 0;
}