#include "tcp_server.h"  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "procmsg.h"


CHAR uacPrintBuf[MAXSIZE];
WORD32 dwPrintlen = 0;

char uacRecvBuf[MAXSIZE] = {0};  
char uacExecCmd[MAXSIZE] = {0};  


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

	// printf("htonll : %016llx\n", u64_net);
	return u64_net;
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

// 设置一个文件描述符为nonblock  
int set_nonblocking(int fd)  
{  
    int flags;  
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)  
        flags = 0;  
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);  
}  

int set_blocking(int fd)  
{  
    int flags;  
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)  
        flags = 0;  
    return fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);  
}  


// int sendmsg(int socket, char *buf, unsigned int count)
// {
	// char *msg = (char *)malloc(count);
	// if(!msg)
		// return -1;
	// set_blocking(socket);
	// memset(msg, 0, count);
	// memcpy(msg, buf, count);

	// // int n = write(socket, msg,count);	
	// int n = send(socket, msg,count,0);	
	// printf("write socket len:%d, todo len:%d\n", n, count);
	// free(msg);
	// // usleep(1000);
	// // sleep(1);
	// return 0;
// }

// int writecnta(int &sock,char *buf)
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

  


tcp_server::tcp_server(long long listen_port) 
{  

	printf("server startup! listen max:%u\n", SOMAXCONN);
	m_server_sockfd = socket(AF_INET, SOCK_STREAM, 0);//建立服务器端socket 
	memset(&m_server_address,0,sizeof(m_server_address));  
	m_server_address.sin_family = AF_INET; 
	m_server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
	m_server_address.sin_port = htons(listen_port); 
	m_server_len = sizeof(m_server_address); 

	int on = 1;
    if (setsockopt(m_server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
   	{
        perror("setsockopt\n");
		exit(0);
	}

	if( bind(m_server_sockfd,(sockaddr*) &m_server_address,sizeof(m_server_address)) < 0 ) 
	{  
        perror("bind\n");
	}  

	if( listen(m_server_sockfd,SOMAXCONN) < 0 ) 
	{  
        perror("bind\n");
	}  


	FD_ZERO(&m_readfds); 
	FD_SET(m_server_sockfd, &m_readfds);//将服务器端socket加入到集合中

	m_cltnum = 0;

}  

tcp_server::~tcp_server()
{
	// printf("here is socket closed\n");
	// shutdown(accept_fd, SHUT_RDWR);
	// close(accept_fd); 
	// shutdown(socket_fd, SHUT_RDWR);
	// close(socket_fd); 
}

/**
** 
**  接口消息入口
** 
** 
**/
int tcp_server::recv_msg() 
{
	while(1) 
    {
        int fd; 
        m_testfds = m_readfds;//将需要监视的描述符集copy到select查询队列中，select会对其修改，所以一定要分开使用变量 


		struct stat tStat;


        /*无限期阻塞，并测试文件描述符变动 */
        m_result = select(FD_SETSIZE + 1, &m_testfds, (fd_set *)0,(fd_set *)0, (struct timeval *) 0); //FD_SETSIZE：系统默认的最大文件描述符
		if (-1 == fstat(m_result, &tStat))
		{
			printf("fstat %d error:%s", m_result, strerror(errno));		
		}

        if(m_result < 1) 
        { 
            perror("select failed"); 
            exit(1); 
//			continue;
        } 

        /*扫描所有的文件描述符*/
        for(fd = 0; fd < FD_SETSIZE; fd++) 
        {
            /*找到相关文件描述符*/
            if(FD_ISSET(fd,&m_testfds)) 
            { 
              /*判断是否为服务器套接字，是则表示为客户请求连接。*/
                if(fd == m_server_sockfd) 
                { 
                    m_client_len = sizeof(m_client_address); 
                    m_client_sockfd = accept(m_server_sockfd, (struct sockaddr *)&m_client_address, (socklen_t*)&m_client_len); 
                    FD_SET(m_client_sockfd, &m_readfds);//将客户端socket加入到集合中
//                    printf("adding client on fd %d\n", m_client_sockfd); 
					printf("[%04u] Received a connection from %s[%u]  ", ++m_cltnum, (char*) inet_ntoa(m_client_address.sin_addr), m_client_address.sin_port);  
					fflush(stdout);
                } 
                /*客户端socket中有数据请求时*/
                else 
                { 
					procclt(fd);
                } 
            } 
        } 
    } 
	
	return 0;  
}  


void tcp_server::do_service(int conn)
{
	int offset = OFFSET_HEAD;
	int status;


    while (1)
    {
		memset(uacRecvBuf,0,sizeof(uacRecvBuf));  
		memset(uacExecCmd,0,sizeof(uacExecCmd));  
        int size = read(conn, uacRecvBuf, sizeof(uacRecvBuf));
        if (size == 0)   //客户端关闭了
        {
            printf("client close\n");
            break;
        }
        else if (size == -1)
		{
            perror("read error\n");
			break;
		}
		// showmsg(uacRecvBuf, size);
		memcpy(uacExecCmd, uacRecvBuf + offset, size - offset);
		fprintf(stdout, "uacExecCmd  : %s\n",uacExecCmd);
		fprintf(stdout, "-------------cmd list--------------\n");
		procmsgfromgui(uacExecCmd);
		int status = system(uacExecCmd);
		if(status < 0)
		{
			printf("cmd: %s\t error: %s", uacExecCmd, strerror(errno)); // 这里务必要把errno信息输出或记入Log
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

/**
**  处理客户socket fd
**  接收数据等
**/
int tcp_server::procclt(int fd) 
{
	
	/** 回传数据 **/
	int   fdpipe[2] = {0};
	int   n = 0, count = 0; 
	memset(uacPrintBuf,0,MAXSIZE);  
	pid_t pid = 0;
	int nread = 0;
	
	ioctl(fd, FIONREAD, &nread);//取得数据量交给nread

	/*客户数据请求完毕，关闭套接字，从集合中清除相应描述符 */
	if(nread == 0) 
	{ 
		close(fd); 
		FD_CLR(fd, &m_readfds); //去掉关闭的fd
		printf("removing client on fd %d\n", fd); 
	} 
	/*处理客户数据请求*/
	else 
	{ 
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
			close(m_server_sockfd);
			close(fdpipe[0]);	  /* close read end */
//			setvbuf ( stdout , NULL , _IONBF , 1024 );
//			setvbuf ( stderr , NULL , _IONBF , 1024 );
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
			do_service(fd);
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
			FD_CLR(fd, &m_readfds); //去掉关闭的fd
		}
	} 



	return 0;
}



