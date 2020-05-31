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
#include <pthread.h>
#include "stdtype.h"  

using namespace std;
#define DEFAULT_BIND_PORT 9999
#define DEFAULT_MAXEVENTS 60
#define DEFAULT_TIMEOUT   500
#define OFFSET_HEAD  12
#define MAXSIZE 1 * 512 * 1024

#define MAXLINE 2 * 1024
#define OPEN_MAX 100
//#define LISTENQ 20
#define LISTENQ SOMAXCONN
#define SERV_PORT 5000
#define INFTIM 1000

#define CHECKPOINTER(para, retval)  \
	if(NULL == para)\
	{\
		return retval;\
	}

#define CHECKPOINTERNULL(para)  \
	if(NULL == para)\
	{\
		return;\
	}




#pragma pack(1)
typedef struct{
	char *pcmd;
	int sockfd;
}T_threadpara, *P_threadpara;


typedef struct {  
    unsigned int u32_h;  
    unsigned int u32_l;  
}Int64_t;  
  
typedef union {  
    unsigned long long u64;  
    Int64_t st64;  
}Convert64_t;  
  
  

#pragma pack(0)



CHAR uacPrintBuf[MAXSIZE] = {0};
WORD32 dwPrintlen = 0;

char uacRecvBuf[MAXSIZE] = {0};  
char uacExecCmd[MAXSIZE] = {0};  



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
	
//    fprintf(stderr,"find space pos:%d %d\n",n, buf[n]);
	return n;
}



unsigned long long convert64word(unsigned long long writelen)
{
	unsigned long long u64_host, u64_net;  
	Convert64_t box_in, box_out;  

	box_in.u64 = writelen;  
	box_out.st64.u32_h = htonl(box_in.st64.u32_l);  
	box_out.st64.u32_l = htonl(box_in.st64.u32_h);  
	u64_net = box_out.u64;  

	// printf("htonll : %016llx\n", u64_net);
	return u64_net;
}

// 设置一个文件描述符为nonblock  
int set_nonblocking(int fd)  
{  
    int flags;  
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)  
        flags = 0;  
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);  
}  

void do_service(T_threadpara *ptPara)
{
	char *pcmd = ptPara->pcmd;
	printf("\n");
	printf("==============proc task: [%s] ==============\n", pcmd);
	printf("==============proc task: [%s] ==============\n", pcmd);
	int status = system(pcmd);
	if(status < 0)
	{
		printf("cmd: %s\t error: %s", pcmd, strerror(errno)); // 这里务必要把errno信息输出或记入Log
		// return -1;
		exit(0); 
	}
	if(WIFEXITED(status))
	{
		// printf("normal termination, exit status = %d\n", WEXITSTATUS(status)); //取得cmdstring执行结果 
	}
	else if(WIFSIGNALED(status))
	{
		printf("abnormal termination,signal number =%d\n", WTERMSIG(status)); //如果cmdstring被信号中断，取得信号值
	}
	else if(WIFSTOPPED(status))
	{
		printf("process stopped, signal number =%d\n", WSTOPSIG(status)); //如果cmdstring被信号暂停执行，取得信号值
	}							

}

int sendmsg(int &sock,char *buf, int length)
{
    // int length;
    int wlength;
    int tmpLength;
    tmpLength=0;
	set_nonblocking(sock);
    // length=strlen(buf);
    while(tmpLength != length)
    {
        wlength=write(sock,&buf[tmpLength],length-tmpLength);
        if(wlength < 0)
        {
            if( errno == EINTR )
            {
                wlength=0;
            }
            else
            {
                close(sock);
                return 1;
            }
        }
        else
        {
            tmpLength+=wlength;
			usleep(500000);
        }
    }
	// sleep(1);
	// usleep(800000);
    return 0;
}


int pipedentry(T_threadpara *ptPara)
{
	/** 回传数据 **/
	int   fdpipe[2] = {0};
	int   n = 0, count = 0; 
	pid_t pid = 0;
	int nread = 0;

	int fd = ptPara->sockfd;
	
	if (pipe(fdpipe) < 0)
	  return -1;  
	
	pid = fork();
	if (pid == -1)
	{
		perror("fork error");
		exit(0);
	}
	if (pid == 0)
	{
		// 子进程
//		close(m_server_sockfd);
		close(fdpipe[0]);	  /* close read end */
		setvbuf ( stdout , NULL , _IONBF , 1024 );
		setvbuf ( stderr , NULL , _IONBF , 1024 );
		if (fdpipe[1] != STDOUT_FILENO)
		{
		  if (dup2(fdpipe[1], STDOUT_FILENO) != STDOUT_FILENO)
		  {
			  return -1;
		  }
		  if (dup2(fdpipe[1], STDERR_FILENO) != STDERR_FILENO)
		  {
			  return -1;
		  }
		  close(fdpipe[1]);
		}				
		do_service(ptPara);
		/* restore original standard output
		  handle */
	   close(fdpipe[1]);
	   /* close duplicate handle for STDOUT */
	   // close(oldstdout); 			
	   exit(EXIT_SUCCESS);
		
	}
	else
	{
		close(fdpipe[1]);	  /* close write end */
		count = 0;
		memset(uacPrintBuf,0 ,sizeof(uacPrintBuf));
		dwPrintlen = sizeof(uacPrintBuf);
		const int wpos = 8;
		
		while ((n = read(fdpipe[0], uacPrintBuf + wpos, dwPrintlen)) > 0)
		{
			count += n;
			WORD64 writelen = n;
			WORD32 sendsum = n + 8;
			writelen = convert64word(n);
			memcpy(uacPrintBuf, &writelen, sizeof(writelen));
			sendmsg(fd, uacPrintBuf,sendsum);
			//clear buf
//				memset(uacPrintBuf,0 ,sizeof(uacPrintBuf)); 				
		}
		close(fdpipe[0]);
		printf("out msg len :%d %0.1fK %0.1fM\n", count, (float)(count/1024), (float)(count/1024.0/1024.0));
		close(fd); //父进程
	}

	return 0;
}

void cmdentry(void *para)
{
    int i = 0;
	T_threadpara *pInPara = (T_threadpara *)para;
	CHECKPOINTERNULL(pInPara);

	char *buf  = pInPara->pcmd;
	int sockfd = pInPara->sockfd;
	int execlen = strlen(buf);
	printf("in task[%u] cmd:%s[%d]\n", pthread_self(), buf, execlen);
#if 0
    while (i < 10) {
        i+=1;
        sleep(1);
		printf("exec cmd:%s[%d]\n", buf, execlen);
    }
#else
	pipedentry(pInPara);

#endif
	printf("in task[%u] cmd:%s[%d] end!!\n", pthread_self(), buf, execlen);

/**
**  释放资源
** 
**/
    free(pInPara->pcmd);
    free(pInPara);
    pthread_detach(pthread_self());
}


int execcmd(char *buf, int sockfd)
{
	int execlen = strlen(buf);

	if(0 == execlen)
	{
		printf("!!cmd empty, no deal!!\n");
		return -2;
	}

	printf("exec cmd:%s[%d] for fd:%d\n", buf, execlen, sockfd);
	T_threadpara *para = (T_threadpara *)malloc(sizeof(*para));
	CHECKPOINTER(para, -1);
	memset(para, 0, sizeof(*para));
	
	char *pcmd = (char *)malloc(execlen + 1);
	CHECKPOINTER(pcmd, -1);
	memset(pcmd, 0, execlen + 1);
	memcpy(pcmd, buf, execlen);

	para->pcmd = pcmd;
	para->sockfd = sockfd;

/**
** 
** 创建任务
** 
**/
	//线程开始的地址	
	int ret;
	pthread_t star_location;
	ret = pthread_create(&star_location, NULL,(void* (*)(void*))cmdentry, para);
	if (ret == !0) {
		perror("creat task\n");
	}
   
	return 0;
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

//                cout << "read[" << n << "]" << line + skipspace(line,n) <<endl;
                execcmd(line + skipspace(line,n), sockfd);
                
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
