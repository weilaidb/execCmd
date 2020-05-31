#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

using namespace std;
#define DEFAULT_BIND_PORT 9999
#define DEFAULT_MAXEVENTS 20
#define DEFAULT_TIMEOUT   500


#define MAXLINE 2 * 1024
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

void setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        exit(1);
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        exit(1);
    }
}

int usage(int argc, char* argv[])
{
    int portnumber;
    if ( 2 == argc )
    {
        if( (portnumber = atoi(argv[1])) < 0 )
        {
            fprintf(stderr,"Usage:%s portnumber\n",argv[0]);
            return 1;
        }
    }
    else
    {
    	portnumber = DEFAULT_BIND_PORT;
        fprintf(stderr,"Usage:%s portnumber\n",argv[0]);
        fprintf(stderr,"Using default portnumber:%d\n",portnumber);
    }
	return portnumber;
}


int skipspace(char *buf, int len)
{
	int n = 0;
	int k = 0;
	int i = 0;
	#if 0
	for(n = 0; n <len; n++)
	{
/**
** 0 空字符
** 6 收到通知
** 8 退格
** 22 同步空闲
** 31 单元分隔符
** 32 space
** 37 %
**/
		
		if((0 == (buf[n]))
		|| (6 == (buf[n]))
		|| (8 == (buf[n]))
		|| (22 == (buf[n]))
		|| (31 == (buf[n]))
		|| (32 == (buf[n]))
//		|| (37 == (buf[n]))
		) 
		{
			continue;
		}
		else
		{
			break;
		}
	}
	#else
	for(n = 0; n <len; n++)
	{

		if(!isalpha(buf[n])) 
		{
			continue;
		}
		else
		{
			break;
		}
	}
	#endif
	
    fprintf(stderr,"find space pos:%d %d\n",n, buf[n]);
	return n;
}

int main(int argc, char* argv[])
{
    int i, maxi, listenfd, connfd, sockfd,epfd,nfds, portnumber;
    ssize_t n;
    char line[MAXLINE];
    socklen_t clilen;

	portnumber = usage(argc, argv);

    //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件

    struct epoll_event ev,events[DEFAULT_MAXEVENTS];
    //生成用于处理accept的epoll专用的文件描述符

    epfd=epoll_create(256);
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //把socket设置为非阻塞方式

    //setnonblocking(listenfd);

    //设置与要处理的事件相关的文件描述符

    ev.data.fd=listenfd;
    //设置要处理的事件类型

    ev.events=EPOLLIN|EPOLLET;
    //ev.events=EPOLLIN;

    //注册epoll事件

    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    // char *local_addr="127.0.0.1";
    // inet_aton(local_addr,&(serveraddr.sin_addr));//htons(portnumber);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port=htons(portnumber);
	int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
   	{
        perror("setsockopt\n");
		exit(0);
	}
	
	if( bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr)) < 0 ) 
	{  
        perror("bind\n");
	}  

	if(listen(listenfd, LISTENQ) < 0 ) 
	{  
        perror("bind\n");
	}  
	
    maxi = 0;
    for ( ; ; ) {
        //等待epoll事件的发生

        nfds=epoll_wait(epfd,events,DEFAULT_MAXEVENTS,DEFAULT_TIMEOUT);
        //处理所发生的所有事件
        for(i=0;i<nfds;++i)
        {
            if(events[i].data.fd==listenfd)//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。

            {
                connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
                if(connfd<0){
                    perror("connfd<0");
                    exit(1);
                }
                //setnonblocking(connfd);

                char *str = inet_ntoa(clientaddr.sin_addr);
                cout << "accapt a connection from " << str << "[" << clientaddr.sin_port << "]"<<endl;
				
                //设置用于读操作的文件描述符

                ev.data.fd=connfd;
                //设置用于注测的读操作事件

                ev.events=EPOLLIN|EPOLLET;
                //ev.events=EPOLLIN;

                //注册ev

                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
            }
            else if(events[i].events&EPOLLIN)//如果是已经连接的用户，并且收到数据，那么进行读入。
            {
                cout << "EPOLLIN" << endl;
                if ( (sockfd = events[i].data.fd) < 0)
                    continue;
                if ( (n = read(sockfd, line, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        close(sockfd);
                        events[i].data.fd = -1;
                    } else
                        std::cout<<"readline error"<<std::endl;
                } else if (n == 0) {
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                line[n] = '\0';

                cout << "read[" << n << "]" << line + skipspace(line,n) <<endl;
                //设置用于写操作的文件描述符

                ev.data.fd=sockfd;
                //设置用于注测的写操作事件

                ev.events=EPOLLOUT|EPOLLET;
                //修改sockfd上要处理的事件为EPOLLOUT

                //epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
            }
            else if(events[i].events&EPOLLOUT) // 如果有数据发送
            {
                sockfd = events[i].data.fd;
                write(sockfd, line, n);
                //设置用于读操作的文件描述符
                cout << "write " << line << ", n:" << n <<endl;

                ev.data.fd=sockfd;
                //设置用于注测的读操作事件

                ev.events=EPOLLIN|EPOLLET;
                //修改sockfd上要处理的事件为EPOLIN

                epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
            }
        }
    }
    return 0;
}