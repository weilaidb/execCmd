#include "tcp_server.h"  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <sys/wait.h>

char printbuf[MAXSIZE];
int printlen = 0;

char tmpbuf[MAXSIZE];
int tmplen = 0;


typedef struct {  
    unsigned int u32_h;  
    unsigned int u32_l;  
}Int64_t;  
  
typedef union {  
    unsigned long long u64;  
    Int64_t st64;  
}Convert64_t;  
  
// unsigned long long u64_host, u64_net;  
// Convert64_t box_in, box_out;  
  
// box_in.u64 = u64_host;  
// box_out.st64.u32_h = htonl(box_in.st64.u32_l);  
// box_out.st64.u32_l = htonl(box_in.st64.u32_h);  
// u64_net = box_out.u64;  
  
// printf("htonll : %016llx\n", u64_net);

unsigned long long convert64word(unsigned long long writelen)
{
	unsigned long long u64_host, u64_net;  
	Convert64_t box_in, box_out;  

	box_in.u64 = writelen;  
	box_out.st64.u32_h = htonl(box_in.st64.u32_l);  
	box_out.st64.u32_l = htonl(box_in.st64.u32_h);  
	u64_net = box_out.u64;  

	printf("htonll : %016llx\n", u64_net);
	return u64_net;
}

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

void showmsg(char *str, int len)
{
	int i = 0;
	printf("string list:%p, len:%d\n", str, len);
	printf("1 2 3 4 5 6 7 8 9 10\n");
	for(i = 0; i < len; i++)
	{
		printf("%-02d :%-02x \n",i + 1, (unsigned char)str[i]);
		// printf("%c",str[i]);
	}
	printf("\n");
}
/**
   * 增强的system函数，能够返回system调用的输出
   *
   * @param[in] cmdstring 调用外部程序或脚本的命令串
   * @param[out] buf 返回外部命令的结果的缓冲区
   * @param[in] len 缓冲区buf的长度
   *
   * @return 0: 成功; -1: 失败 
   */
int mysystem(char* cmdstring, char* buf, int len)
{
      int   fd[2];
      pid_t pid;
      int   n, count; 
      memset(buf, 0, len);
      if (pipe(fd) < 0)
          return -1;
      if ((pid = fork()) < 0)
          return -1;
      else if (pid > 0)     /* parent process */
      {
          close(fd[1]);     /* close write end */
          count = 0;
          while ((n = read(fd[0], buf + count, len)) > 0 && count > len)
              count += n;
          close(fd[0]);
          if (waitpid(pid, NULL, 0) > 0)
              return -1;
      }
      else                  /* child process */
      {
          close(fd[0]);     /* close read end */
          if (fd[1] != STDOUT_FILENO)
          {
              if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
              {
                  return -1;
              }
              close(fd[1]);
          } 
          if (execl("/bin/sh", "sh", "-c", cmdstring, (char*)0) == -1)
              return -1;
      } 
      return 0;
}

typedef union
{
	char len[8];
	long long length;
	// char printbuf[1024];
}combinebuf;
  
int tcp_server::recv_msg() 
{  
	int   fd[2];
	int   n, count; 

	memset(printbuf,0,MAXSIZE);  

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
#if 1

			if (pipe(fd) < 0)
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
				close(socket_fd);
				close(fd[0]);     /* close read end */
				if (fd[1] != STDOUT_FILENO)
				{
				  if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO)
				  {
					  return -1;
				  }
				  close(fd[1]);
				} 				
				do_service(accept_fd);
				/* restore original standard output
				  handle */
			   close(fd[1]);
			   /* close duplicate handle for STDOUT */
			   // close(oldstdout);				
			   exit(EXIT_SUCCESS);
				
			}
			else
			{
				close(fd[1]);     /* close write end */
				count = 0;
				memset(printbuf,0 ,sizeof(printbuf));
				printlen = sizeof(printbuf);
				
				
				while ((n = read(fd[0], printbuf + count, printlen)) > 0)
				{
				  count += n;
				  printf("n :%d\n", n);
				  // write(accept_fd, printbuf + count,n);
				}
				close(fd[0]);
				printf("get child out msg len :%d\n", count);
				printf("get child out printbuf:%s\n", printbuf);
				// printf("sizeof(long long):%d\n", sizeof(long long));
				// printf("sizeof( unsigned long long):%d\n", sizeof( unsigned long long));
				// memmove(printbuf + 8, printbuf, count);
				memcpy(tmpbuf, printbuf, sizeof(printbuf));
				memset(printbuf, 0, sizeof(printbuf));
				unsigned long long writelen = count;
				writelen = convert64word(writelen);
				memcpy(printbuf, &writelen, sizeof(writelen));
				
				memcpy(printbuf + sizeof(writelen), tmpbuf, count);
				// *(long long *)printbuf = count;
				// showmsg(printbuf, count + 8);
				write(accept_fd, printbuf,count + 8);
				close(accept_fd); //父进程
			}
#else
			mysystem();
#endif	
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
		// showmsg(recvbuf, size);
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
	// printf("here is socket closed\n");
	// shutdown(accept_fd, SHUT_RDWR);
	// close(accept_fd); 
	// shutdown(socket_fd, SHUT_RDWR);
	// close(socket_fd); 
}


extern "C" void testabc(void)
{
	printf("testabc\n");
}
