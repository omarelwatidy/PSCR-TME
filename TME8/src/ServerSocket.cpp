#include "Socket.h"
#include "ServerSocket.h"
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
namespace pr{

ServerSocket::ServerSocket(int port):socketfd(-1){
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sockfd,(struct sockaddr*)&addr,sizeof(addr)) <0){
		::close(sockfd);
		perror("bind");
		return;
	}
	if (listen(sockfd,10) <0){
		perror("listen");
		return;
	}
	this->socketfd = sockfd;

}
Socket ServerSocket::accept(){
	struct sockaddr_in exp;
	socklen_t sz = sizeof(exp);
	int fdcom = ::accept(this->socketfd,(struct sockaddr*)&exp,&sz);
	if (fdcom < 0){
		perror("accept");
		return Socket(-1);

	}
	std::cout << "Connection de"<< &exp<< std::endl;
return Socket(fdcom);

}
void ServerSocket::close(){
	if (isOpen()){
		::close(socketfd);
		socketfd = -1;
	}

}



}
