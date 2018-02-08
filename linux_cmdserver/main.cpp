#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "tcp_server.h"
#include <sys/wait.h>
#include <sys/types.h>


#define BIND_PORT 99999

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

int main(int argc,char* argv[])  
{  
	signal(SIGINT, Stop); 
	signal(SIGCHLD, handler);

	atexit(atexit_handle_1);  
	atexit(atexit_handle_2);  
		
	printf("hello word\n");
	//tcp_server ts(atoll(argv[1]));  
	
	ts.recv_msg();  
	return 0;  
} 

