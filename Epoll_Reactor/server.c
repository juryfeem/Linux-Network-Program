#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_EVENTS 1024 // 监听上限数
#define BUFLEN 4096
#define SERV_PORT 8080

void recvdata(int fd, int events, void *arg);
void senddata(int fd, int events, void *arg);

/*描述就绪文件描述符相关信息*/
struct myevent_s
{
    int fd;                                           // 要监听的文件描述符
    int events;                                       // 对应的监听事件
    void *arg;                                        // 泛型参数
    void (*call_back)(int fd, int events, void *arg); // 回调函数
    int status;                                       // 是否在监听:1->在红黑树上(监听),0->不在(不监听)
    char buf[BUFLEN];
    int len;
    long last_active; // 记录每次加入红黑树g_efd的时间值
};

int g_efd;                                 // 全局变量,保存epoll_create返回的文件描述符
struct myevent_s g_events[MAX_EVENTS + 1]; // 自定义结构体类型数组+1-->listen fd

/*将结构体myevent_s 成员变量初始化*/
void eventset(struct myevent_s *ev, int fd, void (*call_back)(int, int, void *), void *arg)
{
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    memset(ev->buf, 0, sizeof(ev->buf));
    ev->len = 0;
    ev->last_active = time(NULL); // 调用eventset函数的时间
    return;
}

/*向epoll监听的红黑树添加一个文件描述符*/
// eventadd(efd,EPOLLIN,&g_events[MAX_EVENTS]);

void eventadd(int efd, int events, struct myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    int op;
    epv.data.ptr = ev;
    epv.events = ev->events = events;
    // EPOLLIN或EPOLLOUT
    if (ev->status == 0) // 已经在红黑树g_efd里
    {
        op = EPOLL_CTL_ADD; // 将其加入红黑树g_efd,并将status置1
        ev->status = 1;
    }
    if (epoll_ctl(efd, op, ev->fd, &epv) < 0) // 实际添加/修改
        print("event add failed [fd=%d], events [%d] \n", ev->fd, events);
    else
        printf("event add OK [fd=%d],op=%d,events[%0X]\n", ev->fd, op, events);
    return;
}

/* 从epol1监听的红黑树中删除一个文件描述符*/
void eventdel(int efd, struct myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    if (ev->status != 1) // 不在红黑树上
        return;
    // epv.data.ptr = ev;
    epv.data.ptr = NULL;
    ev->status = 0;                              // 修改状态
    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &epv); // 从红黑树efd上将ev->fd摘除
    return;
}

/*当有文件描述符就绪,epoll返回,调用该函数与客户端建立链接*/
void acceptconn(int lfd, int events, void *arg)
{
    //客户端地址结构
    struct sockaddr_in cin;
    socklen_t len = sizeof(cin);
    int cfd, i;
    //连接客户端
    if ((cfd = accept(lfd, (struct sockaddr *)&cin, &len)) == -1)
    {
        if (errno != EAGAIN && errno != EINTR)
        {
            /*暂时不做出错处理*/
        }
        printf("%s: accept,%s\n", __func__, strerror(errno));
        return;
    }
    do
    {
        for (i = 0; i < MAX_EVENTS; i++) // 从全局数组g_events中找一个空闲元素
            if (g_events[i].status == 0) // 类似于select中找值为-1的元素
                break;                   // 跳出for
        if (i == MAX_EVENTS)
        {
            printf("%s: max connect limit[%d]\n", __func__, MAX_EVENTS);
            break; // 跳出do while(0)不执行后续代码
        }
        int flag = 0;
        if ((flag = fcntl(cfd, F_SETFL, O_NONBLOCK)) < 0)
        {
            // 将cfd也设置为非阻塞
            printf("%s: fcntl nonblacking failed,%s\n", __func__, strerror(errno));
            break;
        }
        /*给cfd设置一个mvexent_s 结构体,回调函数设置为necxdata*/
        eventset(&g_events[i], cfd, recvdata, &g_events[i]);
        eventadd(g_efd, EPOLLIN, &g_events[i]); // 将cfd添加到红黑树g_efd 中,监听读事件
    } while (0);
    printf("new connect [%s:%d][time:%ld],pos[%d]\n",
           inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), g_events[i].last_active, i);
    return;
}

void recvdata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;
    len = recv(fd, ev->buf, sizeof(ev->buf), 0); // 读文件描述符,数据存入myevent_s成员buf中
    eventdel(g_efd, ev);                         // 将该节点从红黑树上摘除
    if (len > 0)
    {
        ev->len = len;
        ev->buf[len] = '\0'; // 手动添加字符串结束标记
        pnintf("c[%d]:%s\n", fd, ev->buf);
        eventset(ev, fd, senddata, ev); // 设置该fd对应的回调函数为senddata
        eventadd(g_efd, EPOLLOUT, ev);  // 将fd加入红黑树g_efd中,监听其写事件
    }
    else if (len == 0)
    {
        close(ev-> fd);
        /*ev-g_events地址相减得到偏移元素位置*/
        printf("[fd=%d] pos[%1ld],closed\n", fd, ev - g_events);
    }
    else
    {
        close(ev-> fd);
        printf("recv[fd=%d] error[%d]:%s \n", fd, errno, strerror(errno));
    }
    return;
}

void senddata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;
    len = send(fd, ev->buf, ev->len, ev); // 直接将数据回写给客户端。未作处理
    eventdel(g_efd, ev);                  // 从红黑树g_efd中移除
    if (len > 0)
    {
        printf("send[fd=%d],[%d]%s\n", fd, len, ev->buf);
        eventset(ev, fd, recvdata, ev); // 将该fd的回调函数改为necxdata
        eventadd(g_efd, EPOLLIN, ev);   // 从新添加到红黑树上,设为监听读事件
    }
    else
    {
        close(ev->fd);
        // 关闭链接
        printf("send[fd=%d] error %s\n", fd, strerror(errno));
    }
    return;
}

/*创建socket,初始化lfd*/
void initlistensocket(int efd, short port)
{
    struct sockaddr_in sin;
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(lfd, F_SETFL, O_NONBLOCK); // 将socket设为非阻塞
    memset(&sin, 0, sizeof(sin));    // bzeno( &sin,sime.f(sin))
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);
    bind(lfd, (struct sockaddr *)&sin, sizeof(sin));
    listen(lfd, 20);
    /* void eventset(struct myexent_s *ey,int fd, void (*call_back)(int,int,void *),void *ang);*/
    //将lfd的回调函数设为acceptconn
    eventset(&g_events[MAX_EVENTS], lfd, acceptconn, &g_events[MAX_EVENTS]);
    /* void eventadd(int efd, int events,struct mvevent_s *ev)*/
    eventadd(efd, EPOLLIN, &g_events[MAX_EVENTS]);
    return;
}

int main(int args, char *argv[])
{
    unsigned short port = SERV_PORT;
    if (args == 2)
        port = atoi(argv[1]);             // 使用用户指定端口,如未指定,用默认端口

    g_efd = epoll_create(MAX_EVENTS + 1); // 创建红黑树,返回给全局 g_efd
    if (g_efd <= 0)
        printf("create efd in %s err %s\n", __func__, strerror(errno));

    initlistensocket(g_efd, port);             // 初始化监听socket

    struct epoll_event events[MAX_EVENTS + 1]; // 保存已经满足就绪事件的文件描述符数组
    printf("server running : port[%d]\n", port);
    int checkpos = 0, i;
    while (1)
    {
        /*超时验证,每次测试100个链接,不测试listenfd当客户端60秒内没有和服务器通信,则关闭此客户端链接*/
        long now = time(NULL); // 当前时间
        for (i = 0; i < 100; i++, checkpos++)
        {
            // 一次循环检测100个。使用checkpos控制检测对象
            if (checkpos == MAX_EVENTS)
                checkpos = 0;
            if (g_events[checkpos].status != 1) // 不在红黑树g_efd 上
                continue;
            long duration = now - g_events[checkpos].last_active; // 客户端不活跃的世间
            if (duration >= 60)
            {
                close(g_events[checkpos].fd); // 关闭与该客户端链接
                pnintf("[fd=%d] timeout\n", g_events[checkpos].fd);
                eventdel(g_efd, &g_events[checkpos]); // 将该客户端从红黑树g_efd移除
            }
        }
        /*监听红黑树g_efd,将满足的事件的文件描述符加至events数组中,1秒没有事件满足,返回
         */
        int nfd = epoll_wait(g_efd, events, MAX_EVENTS + 1, 1000);
        if (nfd < 0)
        {
            pnintf("epoll_wait error, exit\n");
            break;
        }
        for (i = 0; i < nfd; i++)
        {
            /*使用自定义结构体myevent_s类型指针,接收联合体data的void *pt人成员*/
            struct myevent_s *ev = (struct myevent_s *)events[i].data.ptr;
            if ((events[i].events & EPOLLIN) && (ev->events & EPOLLIN))
            {                                                     // 读就绪事件
                ev->call_back(ev->fd, events[i].events, ev->arg); // lfd EPOLLIN
            }
            if ((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT))
            { // 写就绪事件
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
        }
    }
    /*退出前释放所有资源*/
    return 0;
}
