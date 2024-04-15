//初始化socket
int init_listen_fd(int port, int epfd);

//接收连接
void do_accept(int lfd, int epfd);

//断开连接
void disconnect(int cfd, int epfd);

// 获取一行\r\n结尾的数据
int get_line(int cfd, char *buf, int size);

// 通过文件名获取文件的类型
const char *get_file_type(const char *name);

// 发响应头
void send_respond(int cfd, int no, char *disp, const char *type, int len);

//发回错误
void send_error(int cfd, int no, char *disp);

//发回文件
void send_file(int cfd, const char *file);

//发目录
void send_dir(int cfd, const char *dir);

// 处理http请求
void http_request(int cfd, char *path);

//读
void do_read(int cfd, int epfd);

//epoll启动
void epoll_run(int port);

// 16进制汉转化为10进制
int hexit(char c);

//url编码
void encode_str(char *to, int tosize, const char *from);

//url解码
void decode_str(char *to, char *from);