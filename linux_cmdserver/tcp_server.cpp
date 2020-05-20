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

	// printf("htonll : %016llx\n", u64_net);
	return u64_net;
}

tcp_server::tcp_server(long long listen_port) 
{  

	printf("server startup! listen max:%u\n", SOMAXCONN);
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);//������������socket 
	memset(&server_address,0,sizeof(server_address));  
	server_address.sin_family = AF_INET; 
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); 
	server_address.sin_port = htons(listen_port); 
	server_len = sizeof(server_address); 

	int on = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
   	{
        perror("setsockopt\n");
		exit(0);
	}

	if( bind(server_sockfd,(sockaddr*) &server_address,sizeof(server_address)) < 0 ) 
	{  
        perror("bind\n");
	}  

	if( listen(server_sockfd,SOMAXCONN) < 0 ) 
	{  
        perror("bind\n");
	}  


	FD_ZERO(&readfds); 
	FD_SET(server_sockfd, &readfds);//����������socket���뵽������

	
//	if(( socket_fd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 ){  
//			throw "socket() failed";  
//	}  

//	memset(&myserver,0,sizeof(myserver));  
//	myserver.sin_family = AF_INET;  
//	myserver.sin_addr.s_addr = htonl(INADDR_ANY);  
//	myserver.sin_port = htons(listen_port);  

//	int on = 1;
//    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
//        throw ("setsockopt error");
//	
//	// bool bDontLinger = false;
//	// setsockopt (socket_fd,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(bool));

//	if( bind(socket_fd,(sockaddr*) &myserver,sizeof(myserver)) < 0 ) {  
//			throw "bind() failed";  
//	}  

//	if( listen(socket_fd,SOMAXCONN) < 0 ) {  
//			throw "listen() failed";  
//	}  
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
   * ��ǿ��system�������ܹ�����system���õ����
   *
   * @param[in] cmdstring �����ⲿ�����ű������
   * @param[out] buf �����ⲿ����Ľ���Ļ�����
   * @param[in] len ������buf�ĳ���
   *
   * @return 0: �ɹ�; -1: ʧ�� 
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

// ����һ���ļ�������Ϊnonblock  
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

  
int tcp_server::recv_msg() 
{  
#if 1
printf("server waiting\n"); 
//printf("FD_SETSIZE:%u\n", FD_SETSIZE); 


while(1) 
    {
        char ch; 
        int fd; 
        int nread; 
        testfds = readfds;//����Ҫ���ӵ���������copy��select��ѯ�����У�select������޸ģ�����һ��Ҫ�ֿ�ʹ�ñ��� 

		/** �ش����� **/
		int   fdpipe[2] = {0};
		int   n = 0, count = 0; 
		memset(printbuf,0,MAXSIZE);  
		pid_t pid = 0;


		struct stat tStat;


        /*�������������������ļ��������䶯 */
        result
 = select(FD_SETSIZE, &testfds, (fd_set *)0,(fd_set *)0, (struct timeval *) 0); //FD_SETSIZE��ϵͳĬ�ϵ�����ļ�������
		if (-1 == fstat(result, &tStat))
		{
			printf("fstat %d error:%s", result, strerror(errno));		
		}

        if(result < 1) 
        { 
            perror("server5"); 
            exit(1); 
//			continue;
        } 

        /*ɨ�����е��ļ�������*/
        for(fd = 0; fd < FD_SETSIZE; fd++) 
        {
            /*�ҵ�����ļ�������*/
            if(FD_ISSET(fd,&testfds)) 
            { 
              /*�ж��Ƿ�Ϊ�������׽��֣������ʾΪ�ͻ��������ӡ�*/
                if(fd == server_sockfd) 
                { 
                    client_len = sizeof(client_address); 
                    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, (socklen_t*)&client_len); 
                    FD_SET(client_sockfd, &readfds);//���ͻ���socket���뵽������
//                    printf("adding client on fd %d\n", client_sockfd); 
					printf("Received a connection from %s[%u]\n",(char*) inet_ntoa(client_address.sin_addr), client_address.sin_port);  
                } 
                /*�ͻ���socket������������ʱ*/
                else 
                { 
                    ioctl(fd, FIONREAD, &nread);//ȡ������������nread
                    
                    /*�ͻ�����������ϣ��ر��׽��֣��Ӽ����������Ӧ������ */
                    if(nread == 0) 
                    { 
                        close(fd); 
                        FD_CLR(fd, &readfds); //ȥ���رյ�fd
                        printf("removing client on fd %d\n", fd); 
                    } 
                    /*����ͻ���������*/
                    else 
                    { 
#if 0
                        read(fd, &ch, 1); 
                        sleep(1); 
                        printf("serving client on fd %d\n", fd); 
                        ch++; 
                        write(fd, &ch, 1); 

#else
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
							// �ӽ���
							close(server_sockfd);
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
							memset(printbuf,0 ,sizeof(printbuf));
							printlen = sizeof(printbuf);
							const int wpos = 8;
							
							while ((n = read(fdpipe[0], printbuf + wpos, printlen)) > 0)
							{
								count += n;
								// printf("n :%d\n", n);
								// printf("in get child out printbuf:%s\n", printbuf + wpos);
								unsigned long long writelen = n;
								unsigned int sendsum = n + 8;
								writelen = convert64word(n);
								memcpy(printbuf, &writelen, sizeof(writelen));
								// write(accept_fdpipe, printbuf,sendsum);
								sendmsg(fd, printbuf,sendsum);
								// usleep(20);
								//clear buf
								memset(printbuf,0 ,sizeof(printbuf));					
							  // write(accept_fdpipe, printbuf + count,n);
							}
							close(fdpipe[0]);
							printf("get child out msg len :%d\n", count);
							// printf("get child out printbuf:%s\n", printbuf);
							close(fd); //������
							FD_CLR(fd, &readfds); //ȥ���رյ�fd

						}
							#endif
						
                    } 
                } 
            } 
        } 
    } 

#else



	int   fdpipe[2] = {0};
	int   n = 0, count = 0; 

	memset(printbuf,0,MAXSIZE);  

	pid_t pid = 0;
	while( 1 ) 
	{  

			socklen_t sin_size = sizeof(struct sockaddr_in);  
			if(( accept_fdpipe = accept(socket_fdpipe,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
			{  
					throw "Accept error!";  
					continue;  
			}  
			printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));  
#if 1

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
				// �ӽ���
				close(socket_fdpipe);
				close(fdpipe[0]);     /* close read end */
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
				do_service(accept_fdpipe);
				/* restore original standard output
				  handle */
			   close(fdpipe[1]);
			   /* close duplicate handle for STDOUT */
			   // close(oldstdout);				
			   exit(EXIT_SUCCESS);
				
			}
			else
			{
				close(fdpipe[1]);     /* close write end */
				count = 0;
				memset(printbuf,0 ,sizeof(printbuf));
				printlen = sizeof(printbuf);
				const int wpos = 8;
				
				while ((n = read(fdpipe[0], printbuf + wpos, printlen)) > 0)
				{
					count += n;
					// printf("n :%d\n", n);
					// printf("in get child out printbuf:%s\n", printbuf + wpos);
					unsigned long long writelen = n;
					unsigned int sendsum = n + 8;
					writelen = convert64word(n);
					memcpy(printbuf, &writelen, sizeof(writelen));
					// write(accept_fdpipe, printbuf,sendsum);
					sendmsg(accept_fdpipe, printbuf,sendsum);
					// usleep(20);
					//clear buf
					memset(printbuf,0 ,sizeof(printbuf));					
				  // write(accept_fdpipe, printbuf + count,n);
				}
				close(fdpipe[0]);
				printf("get child out msg len :%d\n", count);
				// printf("get child out printbuf:%s\n", printbuf);
				close(accept_fdpipe); //������
			}

#else
			mysystem();
#endif	
	}  

#endif
	
	return 0;  
}  

char recvbuf[MAXSIZE] = {0};  
char execcmd[MAXSIZE] = {0};  

void tcp_server::do_service(int conn)
{
	int offset = OFFSET_HEAD;
	int status;


    while (1)
    {
		memset(recvbuf,0,sizeof(recvbuf));  
		memset(execcmd,0,sizeof(execcmd));  
        int size = read(conn, recvbuf, sizeof(recvbuf));
        if (size == 0)   //�ͻ��˹ر���
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
		fprintf(stdout, "execcmd  : %s\n",execcmd);
		fprintf(stdout, "-------------cmd list--------------\n");
		procmsgfromgui(execcmd);
		int status = system(execcmd);
		if(status < 0)
		{
			printf("cmd: %s\t error: %s", execcmd, strerror(errno)); // �������Ҫ��errno��Ϣ��������Log
			// return -1;
			exit(0); 
		}
		if(WIFEXITED(status))
		{
			// printf("normal termination, exit status = %d\n", WEXITSTATUS(status)); //ȡ��cmdstringִ�н�� 
		}
		else if(WIFSIGNALED(status))
		{
			printf("abnormal termination,signal number =%d\n", WTERMSIG(status)); //���cmdstring���ź��жϣ�ȡ���ź�ֵ
		}
		else if(WIFSTOPPED(status))
		{
			printf("process stopped, signal number =%d\n", WSTOPSIG(status)); //���cmdstring���ź���ִͣ�У�ȡ���ź�ֵ
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



