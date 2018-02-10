#include "tcp_server.h"  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>


tcp_server::tcp_server(long long listen_port) 
{  
  
	if(( socket_fd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 ){  
			throw "socket() failed";  
	}  

	memset(&myserver,0,sizeof(myserver));  
	myserver.sin_family = AF_INET;  
	myserver.sin_addr.s_addr = htonl(INADDR_ANY);  
	myserver.sin_port = htons(listen_port);  

	int on = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        throw ("setsockopt error");
	
	// bool bDontLinger = false;
	// setsockopt (socket_fd,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(bool));

	if( bind(socket_fd,(sockaddr*) &myserver,sizeof(myserver)) < 0 ) {  
			throw "bind() failed";  
	}  

	if( listen(socket_fd,SOMAXCONN) < 0 ) {  
			throw "listen() failed";  
	}  
}  

void printbuf(char *str, int len)
{
	int i = 0;
	printf("string list:%p, len:%d\n", str, len);
	printf("1 2 3 4 5 6 7 8 9 10\n");
	for(i = 0; i < len; i++)
	{
		printf("%-02d :%c \n",i + 1, str[i]);
		printf("%c",str[i]);
	}
	printf("\n");
}

  
int tcp_server::recv_msg() 
{  
  
	pid_t pid;
	while( 1 ) 
	{  

			socklen_t sin_size = sizeof(struct sockaddr_in);  
			if(( accept_fd = accept(socket_fd,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
			{  
					throw "Accept error!";  
					continue;  
			}  
			printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));  

			pid = fork();
			if (pid == -1)
			{
				perror("fork error");
				exit(0);
			}
			if (pid == 0)
			{
				// 子进程
				close(socket_fd);
				do_service(accept_fd);
				exit(EXIT_SUCCESS);
			}
			else
				close(accept_fd); //父进程
	}  
	return 0;  
}  

char recvbuf[MAXSIZE];  
char execcmd[MAXSIZE];  

void tcp_server::do_service(int conn)
{
	int offset = 12;
	int status;


    while (1)
    {
		memset(recvbuf,0,MAXSIZE);  
		memset(execcmd,0,MAXSIZE);  
        int size = read(conn, recvbuf, sizeof(recvbuf));
        if (size == 0)   //客户端关闭了
        {
            printf("client close\n");
            break;
        }
        else if (size == -1)
		{
            perror("read error");
			break;
		}
		// printbuf(recvbuf, size);
		memcpy(execcmd, recvbuf + offset, size - offset);
		printf("execcmd  : %s\n",execcmd);
		printf("-------------cmd list--------------\n");
		int status = system(execcmd);
		if(status < 0)
		{
			printf("cmd: %s\t error: %s", execcmd, strerror(errno)); // 这里务必要把errno信息输出或记入Log
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
		break;
    }

}


tcp_server::~tcp_server()
{
	shutdown(accept_fd, SHUT_RDWR);
	close(accept_fd); 
	// shutdown(socket_fd, SHUT_RDWR);
	// close(socket_fd); 
}


extern "C" void testabc(void)
{
	printf("testabc\n");
}
