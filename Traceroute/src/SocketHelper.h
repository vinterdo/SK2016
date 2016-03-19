#ifndef __SOCKHELP_H_
#define __SOCKHELP_H_

#include <arpa/inet.h>

class SocketHelper
{
public:
  static int Socket(int family, int type, int protocol);
  static void Bind(int fd, const struct sockaddr_in *sa, socklen_t salen);
  static ssize_t Recvfrom (int fd, void *ptr, size_t nbytes, int flags, struct sockaddr_in *sa, socklen_t *salenptr);
  static void Sendto(int fd, const void *ptr, int nbytes, int flags, const struct sockaddr_in *sa, socklen_t salen);
  static void Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
  static int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

};
#endif
