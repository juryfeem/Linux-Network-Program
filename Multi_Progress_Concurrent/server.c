#define _XOPEN_SOURCE 700
#include"wrap.h"
#include<signal.h>
#include<string.h>
#include<sys/wait.h>


#define SERV_PORT 10087

//回收子进程
void catch_child(int signum){
    while(waitpid(0, NULL, WNOHANG) > 0);
    return;
}

int main(){
    int lfd, cfd;
    pid_t pid;
    int ret;
    char buf[BUFSIZ];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);

    lfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(lfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

    Listen(lfd, 128);

    //注册信号
    struct sigaction act;
    act.sa_handler = catch_child;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0; 
    ret = sigaction(SIGCHLD, &act, NULL);
    if(ret != 0){
        perr_exit("sigaction error");
    }

    //多进程socket
    while(1){
        cfd = Accept(lfd, (struct sockaddr *)&cli_addr, &cli_addr_len);

        pid = fork();
        if(pid < 0){
            perr_exit("fork error");
        }else if(pid == 0){
            //pid = 0 为子进程
            close(lfd);
            break;
        }else{
            close(cfd);
        }
    }

    if(pid == 0){
        while(1){
            ret = Read(cfd, buf, sizeof(buf));
            if(ret == 0){
                close(cfd);
                exit(1);
            }

            for(int i = 0; i < ret;i++){
                buf[i] = toupper(buf[i]);
            }

            write(cfd, buf, ret);
            write(STDOUT_FILENO, buf, ret);
        }
        close(cfd);
    }
}