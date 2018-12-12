#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "tcp_server.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>
#include <pthread.h>
#include<iostream>
#include<string>
using namespace std;


#define BIND_PORT 9999

typedef struct char_print_parms{
	unsigned int outaddr;
	char *destipaddr;
	char *sendmsg;
	unsigned int msglen;
	char *replaceprefix;
	char *diskno;
	char *openfile;
}char_print_parms;



tcp_server ts(BIND_PORT);




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
    // _exit(0);
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
	printf("current path:%s\n", buffer);
	return 0;
}

int getfilepath(char *buffer, char *filepath)
{
	snprintf(buffer, MAXPATH, "%s", filepath);
	printf("current path:%s\n", buffer);
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
	else if(
	( 0 == strcmp("notepad++", *sendmsg) )
	||( 0 == strcmp("NOTEPAD++", *sendmsg) )
	||( 0 == strcmp("notepad", *sendmsg) )
	||( 0 == strcmp("NOTEPAD", *sendmsg) )
	)
	{
//		printf("special deal pwd\n");
		getcurrentpath(dirpathbuffer);
		if(openfile)
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
		else
		{

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
    while(1){
        fputc('x',stderr);
    }
    return NULL;
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
	servaddr.sin_port = htons(BIND_PORT); //服务器端口
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

	specialprocmsg(&sendmsg, &msglen, replaceprefix, diskno, openfile);

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
	printf("send result:%u\n", ret);
	sleep(1);
	free(pbuffer);

	close(sockfd);


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
	char * ipaddr = argv[1];
	char * replaceprex = argv[2];
	char * panfu = argv[3];
	char * testmsg = argv[4];
	char * openfile = argv[5];
	//		sendmsg2addr(0, ipaddr, testmsg, strlen(testmsg), replaceprex, panfu);
	pthread_t thread1_id;
	struct char_print_parms thread1_args = {0};
	thread1_args.outaddr = 0;
	thread1_args.destipaddr = ipaddr;
	thread1_args.sendmsg = testmsg;
	thread1_args.msglen = strlen(testmsg);
	thread1_args.replaceprefix = replaceprex;
	thread1_args.diskno = panfu;
	thread1_args.openfile = openfile;
	pthread_create(&thread1_id,NULL,&sendmsg2addr,&thread1_args);
	pthread_join(thread1_id,NULL);

}

int processCmd(int argc,char* argv[])
{
	switch(argc)
	{
	case 5:
	case 6:
		processCmdPwd(argc, argv);
		break;
	default:
		break;
	}
	exit(0);
}

int main(int argc,char* argv[])
{
	printf("supprt send msg 2 window server\n"
	"usage:./a.out 192.168.1.1 replaceprefix X: orgmsg...\n", argc);
	processCmd(argc, argv);

	signal(SIGINT, Stop);
	signal(SIGCHLD, handler);

	atexit(atexit_handle_1);
	atexit(atexit_handle_2);

//	printf("hello word\n");
	//tcp_server ts(atoll(argv[1]));

	ts.recv_msg();
	return 0;
}

