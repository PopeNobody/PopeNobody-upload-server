#include "checkret.hh"
#include "md5.h"

using namespace checkret;

long min(long lhs, long rhs) {
  return lhs<rhs ? lhs : rhs;
};
bool forking(){
  return false;
};;
int ifd=-1;
int ofd=-1;

char buffer[1024*300];
int main(int argc, char**argv){
  ifd=xopenat(AT_FDCWD,"auth.log",O_RDONLY);
  int bytes=xread(ifd,buffer,sizeof(buffer),true);

  char md5out[128/8];
  memset(md5out,0,sizeof(md5out));
  
  __md5_buffer(buffer,bytes,md5out);
  for(int i=0;i<sizeof(md5out);i++){
    int pair=md5out[i]&0xff;
    dprintf(1,"%02x",pair);
  };
  dprintf(1,"\n");
  int sock=xsocket(AF_INET,SOCK_STREAM,0);
  int res; 
  sockaddr_in sin_addr;
  memset(&sin_addr,0,sizeof(sin_addr)); 
  sin_addr.sin_family = AF_INET;
  socklen_t len=sizeof(sin_addr);
  res = xinet_aton("34.209.78.80", &sin_addr.sin_addr);
  int port = 3333;
  sin_addr.sin_port = htons(port);
  sin_addr.sin_family = AF_INET;
  xconnect(sock,(sockaddr*)&sin_addr,len);
  int flag=1;
  xsetsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
  printf("new connection is on fd %d\n", res);
  int write1 = xwrite(sock,"test.txt\n",9,true);
  char buffer2[2];
  int read1 = xread(sock,buffer2,1);
  dprintf(sock,"%d\n",bytes);
  read1=xread(sock,buffer2,1);
  for(int i=0;i<sizeof(md5out);i++){
    dprintf(sock,"%02x",0xff&md5out[i]);
  };
  dprintf(sock,"\n");
  read1=xread(sock,buffer2,1);
  int write2 = xwrite(sock,buffer,bytes,true);
  printf("last: %10d, %10d, %10d\n", write1, read1, write2);
  return 0;
};
