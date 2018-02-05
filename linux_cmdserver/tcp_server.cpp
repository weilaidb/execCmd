#include "tcp_server.h"  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>


tcp_server::tcp_server(long long listen_port) 
{  
  
	if(( socket_fd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 ){  
			throw "socket() failed";  
	}  

	memset(&myserver,0,sizeof(myserver));  
	myserver.sin_family = AF_INET;  
	myserver.sin_addr.s_addr = htonl(INADDR_ANY);  
	myserver.sin_port = htons(listen_port);  

	// setsockopt(socket_fd,SOL_SOCKET,SO_REUSERADDR,&myserver,sizeof(myserver));
	bool bReuseaddr=true;
	setsockopt (socket_fd,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(bool));

	// bool bDontLinger = false;
	// setsockopt (socket_fd,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(bool));

	if( bind(socket_fd,(sockaddr*) &myserver,sizeof(myserver)) < 0 ) {  
			throw "bind() failed";  
	}  

	if( listen(socket_fd,10) < 0 ) {  
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
		// printf("%-02d :%c \n",i + 1, str[i]);
		printf("%c ",str[i]);
	}
	printf("\n");
}

  
int tcp_server::recv_msg() 
{  
  
	int offset = 12;
	while( 1 ) 
	{  

			socklen_t sin_size = sizeof(struct sockaddr_in);  
			if(( accept_fd = accept(socket_fd,(struct sockaddr*) &remote_addr,&sin_size)) == -1 )  
			{  
					throw "Accept error!";  
					continue;  
			}  
			printf("Received a connection from %s\n",(char*) inet_ntoa(remote_addr.sin_addr));  

			if( !fork() ) {  
					char buffer[MAXSIZE];  
					char execcmd[MAXSIZE];  
					memset(buffer,0,MAXSIZE);  
					memset(execcmd,0,MAXSIZE);  
					int size = read(accept_fd,buffer,MAXSIZE);
					if(size < 0 ) {  
							throw("Read() error!");  
					} else {  
							printbuf(buffer, size);
							printf("Received size   : %d\n",size);  
							printf("Received message: %s\n",buffer);
							memcpy(execcmd, buffer + offset, size - offset);
							printf("execcmd         : %s\n",execcmd);
							system(execcmd);
							break;  
					}  
					exit(0);  
			}  
			close(accept_fd);  
	}  
	return 0;  
}  
