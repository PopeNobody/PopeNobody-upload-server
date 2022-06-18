#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int xsocket(int family, int type, int prot){
  int res=socket(family,type,prot);
  if(res<0) {
    perror("socket");
    exit(1);
  };
  return res;
};
int xbind(int fd, const struct sockaddr *addr, socklen_t len){
  int res=bind(fd,addr,len);
  if(res<0) {
    perror("bind");
    exit(2);
  };
  return res;
};
int xinet_aton(const char *cp, struct in_addr *inp){
  int res=inet_aton(cp,inp);
  if(!res) {
    perror("inet_aton");
    exit(3);
  };
  printf("%s\n",inet_ntoa(*inp));
  return res;
};
int xlisten(int sock, int backlog){
  int res = listen(sock,backlog);
  if(res){
    perror("listen");
    exit(4);
  };
  return 0;
};
int xaccept(int sock, sockaddr *addr, socklen_t *addrlen){
  int res = accept(sock,addr,addrlen);
  if(res<=0){
    perror("accept");
    exit(4);
  };
  return res;
}
int xaccept4(int sock, sockaddr *addr, socklen_t *addrlen,int flags){
  int res = accept4(sock,addr,addrlen,flags);
  if(res<=0){
    perror("accept4");
    exit(4);
  };
  return res;
}
int main(int argc, char**argv){
  int sock=xsocket(AF_INET,SOCK_STREAM,0);
  int res; 
  sockaddr_in sin_addr;
  memset(&sin_addr,0,sizeof(sin_addr)); 
  res = xinet_aton("34.209.78.80",&sin_addr.sin_addr);
  sin_addr.sin_port = htons(3333);
  sin_addr.sin_family = AF_INET;
  size_t len=sizeof(sin_addr);
  res=xbind(sock,(sockaddr*)&sin_addr,len);  
  res=xlisten(sock,1);
  socklen_t socklen;
  int flags=SOCK_NONBLOCK ;
  res=xaccept4(sock,(sockaddr*)&sin_addr,&socklen,flags);
  printf("sock: %d, size: %d\n",sock,socklen);
  return 0;
};
