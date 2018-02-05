#include <unistd.h>  
#include <iostream>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
  
#define MAXSIZE 2048  
  
class tcp_server  
{  
private:  
        int socket_fd,accept_fd;  
        sockaddr_in myserver;  
        sockaddr_in remote_addr;  
  
public:  
        tcp_server(long long listen_port);  
        int recv_msg();  
};
