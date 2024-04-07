#include"wrap.h"

//服务器端口
#define SERV_PORT 10086

int main(){
    int lfd = 0, cfd = 0;
    int ret;
    char buf[BUFSIZ], client_IP[1024];

    //创建socket
    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    //绑定IP和端口
    struct sockaddr_in serv_addr, cli_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(lfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    //设置监听上限
    listen(lfd, 128);
    
    //等待客户端连接
    socklen_t cli_addr_len = sizeof(cli_addr);
    cfd = accept(lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
    printf("client ip:%s port:%d\n", inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(cli_addr.sin_port));
     
    while(1){
    //读数据
    ret = read(cfd, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, ret);
    for(int i = 0; i < ret; i++){
        buf[i] = toupper(buf[i]);
    }

    //写数据
    write(cfd, buf, ret);

    }

    //关闭socket
    close(lfd);
    close(cfd);
    
    return 0;
}