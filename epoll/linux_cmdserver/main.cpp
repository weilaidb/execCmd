#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "stdtype.h"  



using namespace std;
#define DEFAULT_BIND_PORT 9999
#define DEFAULT_MAXEVENTS 60
#define DEFAULT_TIMEOUT   500
#define OFFSET_HEAD  12
#define MAXSIZE 1 * 512 * 1024

#define MAXLINE 2 * 1024
//#define OPEN_MAX 100
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


	
	
#define PARA_NULL_RETNULL(A) \
	if(NULL == A)\
	{\
		printf("para null:%s\n", #A);\
		return NULL;\
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
  
  
typedef struct char_print_parms{
	unsigned int outaddr;
	char *destipaddr;
	char *sendmsg;
	unsigned int msglen;
	char *replaceprefix;
	char *diskno;
	char *openfile;
	char **argv;
	int argc;
}char_print_parms;

#pragma pack(0)



CHAR uacPrintBuf[MAXSIZE] = {0};
WORD32 dwPrintlen = 0;



void handler(int sig)
{
    /*  wait(NULL); //只能等待第一个退出的子进程 */

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}


static void atexit_handle_1(void)
{
        //printf("atexit_handle_1\n");
        printf("==============1\n");
}

static void atexit_handle_2(void)
{
        //printf("atexit_handle_2\n");
        printf("==============2\n");
}

void Stop(int signo)
{
    printf("oops! stop!!!\n");
	// ts.~tcp_server();
    _exit(0);
}

typedef struct T_MsgStruct
{
	unsigned long long mlen;
	char mbuf[0];
}T_MsgStruct;

unsigned long long
htonll(unsigned long long host)
{
        unsigned long temp_low, temp_high;
        temp_low = htonl((long)host);
        temp_high = htonl((long)(host >> 32));

        host &= 0;
        host |= temp_low;
        host <<= 32;
        host |= temp_high;
        return host;
}

#define MAXPATH (1024)
char dirpathbuffer[MAXPATH] = {0};
char notepadpathbuffer[MAXPATH] = {0};
int getcurrentpath(char *buffer)
{
	getcwd(buffer, MAXPATH);
	// printf("current path:%s\n", buffer);
	return 0;
}

int getfilepath(char *buffer, char *filepath)
{
	snprintf(buffer, MAXPATH, "%s", filepath);
	// printf("current path:%s\n", buffer);
	return 0;
}



/* 功  能：将str字符串中的oldstr字符串替换为newstr字符串
 * 参  数：str：操作目标 oldstr：被替换者 newstr：替换者
 * 返回值：返回替换之后的字符串
 * 版  本： V0.2
 */
char *strrpc(char *str,char *oldstr,char *newstr)
{
    char bstr[strlen(str)];//转换缓冲区
    memset(bstr,0,sizeof(bstr));

    for(int i = 0;i < strlen(str);i++){
        if(!strncmp(str+i,oldstr,strlen(oldstr))){//查找目标字符串
            strcat(bstr,newstr);
            i += strlen(oldstr) - 1;
        }else{
        	strncat(bstr,str + i,1);//保存一字节进缓冲区
	    }
    }

    strcpy(str,bstr);
    return str;
}


int specialprocmsg(char **sendmsg, unsigned int *pmsglen
						, char *replaceprefix
						, char *diskno
						, char *openfile
						, int argc
						, char **argv
						)
{

	/** 当前路径时特殊处理 **/
	if(
	( 0 == strcmp("pwd", *sendmsg) )
	||( 0 == strcmp("PWD", *sendmsg) )
	)
	{
//		printf("special deal pwd\n");
		getcurrentpath(dirpathbuffer);
		strrpc(dirpathbuffer, replaceprefix, diskno);
		strrpc(dirpathbuffer, "/", "\\");
//		printf("dirpathbuffer:%s\n", dirpathbuffer);
		*sendmsg = dirpathbuffer;
		*pmsglen = strlen(dirpathbuffer);

	}
	else if(( 0 == strcasecmp("vim", *sendmsg) )
	||( 0 == strcasecmp("notepad++", *sendmsg) )
	||( 0 == strcasecmp("notepad++", *sendmsg) )
	)
	{
//		printf("special deal pwd\n");
		getcurrentpath(dirpathbuffer);
		if('~' == openfile[0])
		{
//			strcat(dirpathbuffer, openfile);
			strrpc(dirpathbuffer, replaceprefix, diskno);
			strrpc(dirpathbuffer, "/", "\\");
			snprintf(notepadpathbuffer, sizeof(notepadpathbuffer)
					, "cmd %s \"%s\\%s\""
					, *sendmsg
					, dirpathbuffer
					, openfile
					);
		}
		else if('/' == openfile[0])
		{
			snprintf(dirpathbuffer, sizeof(dirpathbuffer), "%s", openfile);
//			strcat(dirpathbuffer, openfile);
			strrpc(dirpathbuffer, replaceprefix, diskno);
			strrpc(dirpathbuffer, "/", "\\");
			snprintf(notepadpathbuffer, sizeof(notepadpathbuffer)
					, "cmd %s \"%s\""
					, *sendmsg
					, dirpathbuffer
					);
		}
		else
		{
//			strcat(dirpathbuffer, openfile);
			strrpc(dirpathbuffer, replaceprefix, diskno);
			strrpc(dirpathbuffer, "/", "\\");
			snprintf(notepadpathbuffer, sizeof(notepadpathbuffer)
					, "cmd %s \"%s\\%s\""
					, *sendmsg
					, dirpathbuffer
					, openfile
					);
		}

		//		getfilepath(dirpathbuffer, openfile);

		//		printf("dirpathbuffer:%s\n", dirpathbuffer);
		*sendmsg = notepadpathbuffer;
		*pmsglen = strlen(notepadpathbuffer);

	}
	else if(
	( 0 == strcasecmp("cmd", *sendmsg) )
//	||( 0 == strcmp("NOTEPAD", *sendmsg) )
	)
	{
		int i = 0;
//		printf("No  Content\n");
		snprintf(notepadpathbuffer, sizeof(notepadpathbuffer)
				, "%s "
				, "cmd");

		for(i = 5; i < argc; i++)
		{
			strcat(notepadpathbuffer, " ");
			strcat(notepadpathbuffer, argv[i]);
		}

		//		getfilepath(dirpathbuffer, openfile);

		//		printf("dirpathbuffer:%s\n", dirpathbuffer);
		*sendmsg = notepadpathbuffer;
		*pmsglen = strlen(notepadpathbuffer);

	}
	else
	{
		snprintf(dirpathbuffer, sizeof(dirpathbuffer), "%s", *sendmsg);
		strrpc(dirpathbuffer, "\\", "\*\*");
		*sendmsg = dirpathbuffer;
		*pmsglen = strlen(dirpathbuffer);
	}


	return 0;
}




void* print_xs(void *  unused)
{
    while(1)
    {
        fputc('x',stderr);
    }
    return NULL;
}


void printargs(int argc, char **argv)
{
	int i = 0;
	printf("No  Content\n");
	for(i = 0; i < argc; i++)
	{
		printf("%-03d  %s\n"
			,i+ 1
			,argv[i]
			);
	}

}



void * sendmsg2addr(void* parameter
						)
{
#define MSGSIZEMAX (msglen + sizeof(T_MsgStruct))
#define MSGOFFSET (4)

    struct char_print_parms* p =    (struct char_print_parms*)parameter;
	unsigned int outaddr = p->outaddr;
	char *destipaddr = p->destipaddr;
	char *sendmsg = p->sendmsg;
	unsigned int msglen = p->msglen;
	char *replaceprefix = p->replaceprefix;
	char *diskno = p->diskno;
	char *openfile = p->openfile;
	int argc = p->argc;
	char **argv = p->argv;
//	PARA_NULL_RETNULL(outaddr);
	PARA_NULL_RETNULL(destipaddr);
	PARA_NULL_RETNULL(sendmsg);
	PARA_NULL_RETNULL(replaceprefix);
	PARA_NULL_RETNULL(diskno);
//	PARA_NULL_RETNULL(openfile);
	printargs(argc, argv);

	int sockfd, len;
	//建立socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0
)  //TCP
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET; //IPv4
	if(0 != outaddr )
	{
		servaddr.sin_addr.s_addr = htonl(outaddr); //服务器地址
	}
	else
	{
		servaddr.sin_addr.s_addr = inet_addr(destipaddr); //服务器地址
	}
	servaddr.sin_port = htons(DEFAULT_BIND_PORT); //服务器端口
//	printf("addr:%#x, port:%#x\n", servaddr.sin_addr.s_addr, servaddr.sin_port);

	int connResult = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (connResult < 0)
	{
		printf("连接失败，errno ＝ %d(%s)\n",errno, strerror(errno));
		close(sockfd);
		return NULL;
	}
	else
	{
//		printf("连接成功\n");
	}

	specialprocmsg(&sendmsg, &msglen, replaceprefix, diskno, openfile,argc,argv);

	unsigned char *pbuffer = (unsigned char *)malloc(MSGSIZEMAX);
	if(NULL == pbuffer)
	{
		printf("malloc mem failed\n");
		close(sockfd);
		return NULL;
	}
	memset(pbuffer, 0, MSGSIZEMAX);



	len = msglen;
	T_MsgStruct *ptmp = (T_MsgStruct *)pbuffer;
	ptmp->mlen = htonll(msglen);// + sizeof(T_MsgStruct);
	memcpy(pbuffer + sizeof(T_MsgStruct), sendmsg, len);


	//将字串返回给client 端
	int ret = send(sockfd, pbuffer, MSGSIZEMAX, 0);
	if(ret < 0)
	{
		printf("send result:%u\n", ret);
	}
	//sleep(5);
	free(pbuffer);

	//close(sockfd);


    return NULL;

}

void processCmdPwd(int argc,char* argv[])
{
	//		printf("argc:%u\n", argc);
	//		printf("argv[0]:%s\n", argv[0]);
	//		printf("argv[1]:%s\n", argv[1]);
	//		printf("argv[2]:%s\n", argv[2]);
	//		printf("argv[3]:%s\n", argv[3]);
	//		printf("argv[4]:%s\n", argv[4]);
#define NULLORSELF(A) A?A:0
#define NULLORSELFSTR(A) A?A:""

	char * ipaddr = argv[1];
	char * replaceprex = argv[2];
	char * panfu = argv[3];
	char * testmsg = argv[4];
	char * openfile = argv[5];
	//		sendmsg2addr(0, ipaddr, testmsg, strlen(testmsg), replaceprex, panfu);
	pthread_t thread1_id;
	struct char_print_parms thread1_args = {0};
	thread1_args.argc = argc;
	thread1_args.argv = argv;
	thread1_args.outaddr = 0;
	thread1_args.destipaddr = NULLORSELF(ipaddr);
	thread1_args.sendmsg = NULLORSELF(testmsg);
	thread1_args.msglen = strlen(NULLORSELFSTR(testmsg));
	thread1_args.replaceprefix = NULLORSELF(replaceprex);
	thread1_args.diskno = NULLORSELF(panfu);
	thread1_args.openfile = NULLORSELF(openfile);
	pthread_create(&thread1_id,NULL,&sendmsg2addr,&thread1_args);
	pthread_join(thread1_id,NULL);
	sleep(3);

}

int processCmd(int argc,char* argv[])
{
	switch(argc)
	{
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		// printf("useage:linux_cmdserver  192.168.59.12 /home/weilaidb X: vim filename\n");
		// printf("useage:linux_cmdserver  192.168.59.12 /home/weilaidb X: notepad++ filename\n");
		// printf("useage:linux_cmdserver  192.168.59.12 cmd order\n");
		processCmdPwd(argc, argv);
		exit(0);
		break;
	default:
		break;
	}
}


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
	printf("useage:\n");
	printf("linux_cmdserver  192.168.59.12 /home/weilaidb X: vim filename\n");
	printf("linux_cmdserver  192.168.59.12 /home/weilaidb X: notepad++ filename\n");
	printf("linux_cmdserver  192.168.59.12 cmd order\n");	
	

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
	processCmd(argc, argv);

	signal(SIGINT, Stop);
	signal(SIGCHLD, handler);

	atexit(atexit_handle_1);
	atexit(atexit_handle_2);
	


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
