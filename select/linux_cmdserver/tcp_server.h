#include <unistd.h>  
#include <iostream>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include "stdtype.h"  

#define MAXSIZE 1 * 1024 * 1024
#define OFFSET_HEAD  12

class tcp_server  
{  
private:  
		int m_server_sockfd, m_client_sockfd; 
		int m_server_len, m_client_len; 
		struct sockaddr_in m_server_address; 
		struct sockaddr_in m_client_address; 
		int m_result; 
		fd_set m_readfds, m_testfds; 
/**
** 接收客户端数量
**/
		WORD32 m_cltnum;
		
public:  
        tcp_server(long long listen_port);  
		~tcp_server();
		void do_service(int conn);
        int recv_msg();  
        int procclt(int fd);  
};
