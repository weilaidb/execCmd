#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "tcp_server.h"

#define BIND_PORT 99999

int main(int argc,char* argv[])  
{  
	printf("hello word\n");
	//tcp_server ts(atoll(argv[1]));  
	tcp_server ts(BIND_PORT);  
	ts.recv_msg();  
	return 0;  
} 

