#include "Socket.h"
#include <netdb.h>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
namespace pr {



		std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr){
				char host[1024];
				if(!getnameinfo((sockaddr*)addr,sizeof(struct sockaddr_in),host,1024,NULL,0,0)){
					os << host << std::endl;

				}
				os << inet_ntoa(addr->sin_addr) << std::endl;
					return os;
		}
		void Socket::connect(const std::string& host,int port){
			struct in_addr ip;
			struct addrinfo *res;
			getaddrinfo(host.c_str(),NULL,NULL,&res);
			for (addrinfo*rp = res;rp;rp = rp->ai_next){
				if(rp->ai_family == AF_INET){
					ip = ((struct sockaddr_in*)rp->ai_addr)->sin_addr;
					break;

				}
			}
			freeaddrinfo(res);
		}
		void Socket::connect(struct in_addr ip,int port){
			int fdsock = socket(AF_INET,SOCK_STREAM,0);
			if (fdsock <0){
				perror("socket");return;
			}
			struct sockaddr_in addr;
			addr.sin_family = AF_INET; addr.sin_port = htons(port);
			addr.sin_addr = ip;
			if(::connect(fdsock,(struct sockaddr*)&addr,sizeof(struct sockaddr_in)) <0){
				perror("connect");
				::close(fdsock);
				return;
			}
			this->fd = fdsock;
			return;
		}
		void Socket::close(){
			if  (isOpen()){
				::shutdown(fd,2);
				::close(fd);
				fd = -1;
			}

		}







}

