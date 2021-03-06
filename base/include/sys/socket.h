
#ifndef _SYS_SOCKET_H_
#define _SYS_SOCKET_H_

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "utils.h"

SYS_NS_BEGIN

// socket结果码
enum {
	SOCKET_ERROR = 100,
	SOCKET_CLOSE_ERROR,
	SOCKET_SET_ERROR,
	SOCKET_BIND_ERROR,
	SOCKET_LISTEN_ERROR,
	SOCKET_ACCEPT_ERROR,
	SOCKET_CONNECT_ERROR,
	SOCKET_READ_ERROR,
	SOCKET_WRITE_ERROR,
	SOCKET_TIME_OUT
};


class CSocket
{
public:
    CSocket();
    virtual ~CSocket();
	
	enum SOCKET_STATE
	{
		SOCKET_CLOSE = 0,
		SOCKET_OPEN = 1
	};

    // 创建套接字
    int s_open();
    // 关闭套接字
    int s_close();
	// 关闭调用open返回的套接字
	int s_close(size_t fd);
    // 监听端口,MaxConn为同时处理的最大连接数
    int s_listen();
    // 绑定端口
    int s_bind(IN char* ip, IN int port);
    // 请求建立连接
    int s_connect(IN char* ip, IN int port);
    // 接收连接
    int s_accept(OUT int* fd, OUT struct sockaddr_in* addr);
    // 读取数据
    int s_read(IN int fd, OUT char* buff, OUT int len);
    // 写入数据
    int s_write(IN int fd, IN char* buff);
	
	inline size_t getfd() {return fd_;}
private:
    size_t fd_;
	SOCKET_STATE state_;
};

SYS_NS_END

#endif
