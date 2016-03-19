#include "SocketHelper.h"

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>

int SocketHelper::Socket(int family, int type, int protocol)
{
	int	n;
	if ( (n = socket(family, type, protocol)) < 0)
		std::cout << "cant get socket" << std::endl;
	return n;
}

void SocketHelper::Bind(int fd, const struct sockaddr_in *sa, socklen_t salen)
{
	if (bind(fd, (struct sockaddr*)sa, salen) < 0)
	    std::cout << "cant bind socket" << std::endl;
}

ssize_t SocketHelper::Recvfrom(int fd, void *ptr, size_t nbytes, int flags, struct sockaddr_in *sa, socklen_t *salenptr)
{
	ssize_t	n;
	if ( (n = recvfrom(fd, ptr, nbytes, flags, (struct sockaddr*)sa, salenptr)) < 0)
	  std::cout << "cant recive" << std::endl;
	return n;
}

void SocketHelper::Sendto(int fd, const void *ptr, int nbytes, int flags, const struct sockaddr_in *sa, socklen_t salen)
{
	if (sendto(fd, ptr, nbytes, flags, (struct sockaddr*)sa, salen) != nbytes)
	  std::cout << "cant send to" << std::endl;
}

void SocketHelper::Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
	if (setsockopt(sockfd,level,optname,optval,optlen) < 0) 
	  std::cout << "set socket opt error" << std::endl;
}

int SocketHelper::Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	int n;
	if ((n = select(nfds,readfds,writefds,exceptfds,timeout)) < 0) 
	    std::cout << "cant select from socket" << std::endl;
	return n;	
}
