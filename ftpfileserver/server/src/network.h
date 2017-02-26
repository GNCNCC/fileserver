#ifndef NETWORK_H
#define NETWORK_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdbool.h>
#include<pthread.h>


//创建套接字
int network_socket();
//创建 绑定
int network_server(int port, const char *ip);
//创建 链接服务器
int network_connect(int port, const char *ip);
//接受客户端链接
int network_accept(int sockfd, void *(*task_run)(void *));


#endif//NETWORK_H
