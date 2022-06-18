#ifndef checkret_hh
#define checkret_hh checkret_hh

#include "checkret.hh"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "checkret.hh"
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>

int xsocket(int family, int type, int prot);
int xbind(int fd, const struct sockaddr *addr, socklen_t len);
int xinet_aton(const char *cp, struct in_addr *inp);
int xlisten(int sock, int backlog);
int xaccept(int sock, sockaddr *addr, socklen_t *addrlen);
int xaccept4(int sock, sockaddr *addr, socklen_t *addrlen,int flags);
int xconnect(int sock, sockaddr*addr,socklen_t len);
int xopen(const char *name, int flags);
int xread(int fd, char *buf, size_t size);

#endif
