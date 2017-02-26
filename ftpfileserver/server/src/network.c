#include "network.h"

//创建套接字
int network_socket()
{
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket error");
		return -1;
	}
	return sockfd;
}
//创建 绑定(如果ip为NULL就用默认)
int network_server(int port, const char *ip)
{
	int sockfd = network_socket();
	//地址复用
	int on = 1;
	int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(ret < 0)
	{
		perror("set reuse error");
		return -1;
	}
	
	//2.绑定端口地址
	struct sockaddr_in baddr;
	bzero(&baddr, sizeof(baddr));
	baddr.sin_family = AF_INET ;
	baddr.sin_port = htons(port);
	if(ip == NULL)
	{
		baddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}else
	{
		baddr.sin_addr.s_addr = inet_addr(ip);		
	}
	
	//绑定
	ret = bind(sockfd, (struct sockaddr*)&baddr, sizeof(baddr));
	if(ret < 0)
	{
		perror("bind error");
		return -1;
	}

	ret = listen(sockfd , 5);
	
	return sockfd;
	
}
//创建 链接服务器
int network_connect(int port, const char *ip)
{
	int sockfd = network_socket();	
	//2.绑定端口地址
	struct sockaddr_in baddr;
	bzero(&baddr, sizeof(baddr));
	baddr.sin_family = AF_INET ;
	baddr.sin_port = htons(port);
	baddr.sin_addr.s_addr = inet_addr(ip);		
	
	int ret = connect(sockfd, (struct sockaddr*)&baddr, sizeof(baddr));
	if(ret < 0)
	{
		perror("connect error");
		return -1;
	}
	return sockfd;
}
//接受客户端链接
int network_accept(int sockfd, void *(*task_run)(void *))
{
	struct sockaddr_in cltaddr;
	socklen_t len = sizeof(cltaddr);
	int clientfd = accept(sockfd,(struct sockaddr*)&cltaddr,&len);
	if(clientfd < 0)
	{
		perror("accept error");
		return -1;
	}	
	
	//创建线程
	pthread_t id = 0;
	pthread_create(&id, NULL, task_run, (void*)&clientfd);
	pthread_detach(id);
	return 0;
}
