#include <unistd.h>  
#include <iostream>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
  
#define MAXSIZE 16 * 1024 *1024
#define OFFSET_HEAD  12

class tcp_server  
{  
private:  
		int server_sockfd, client_sockfd; 
		int server_len, client_len; 
		struct sockaddr_in server_address; 
		struct sockaddr_in client_address; 
		int result; 
		fd_set readfds, testfds; 
		
public:  
        tcp_server(long long listen_port);  
		~tcp_server();
		void do_service(int conn);
        int recv_msg();  
};
