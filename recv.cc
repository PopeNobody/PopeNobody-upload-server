#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "checkret.hh"

int main(int argc, char**argv){
  int sock=xsocket(AF_INET,SOCK_STREAM,0);
  int res; 
  sockaddr_in sin_addr;
  memset(&sin_addr,0,sizeof(sin_addr)); 
  res = xinet_aton("0.0.0.0",&sin_addr.sin_addr);
  sin_addr.sin_port = htons(3333);
  sin_addr.sin_family = AF_INET;
  size_t len=sizeof(sin_addr);
  res=xbind(sock,(sockaddr*)&sin_addr,len);  
  res=xlisten(sock,1);
  socklen_t socklen;
  int flags=SOCK_NONBLOCK ;
  int conn=xaccept4(sock,(sockaddr*)&sin_addr,&socklen,flags);
  printf("sock: %d, size: %d\n",sock,socklen);
  char fname[128];
  memset(fname,0,sizeof(fname));
  while(true){
    int pos=0;
    res=xread(conn,fname+pos,1);
    if(res!=1 || fname[pos]=='\n')
      return pos;
  };
  return 0;
};
