#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "fixed_buf.hh"
#include "checkret.hh"
#define announce() 

using namespace ns_checkret;

//dprintf(2,"%s:%d:%s\n",__FILE__,__LINE__,__PRETTY_FUNCTION__);
int ns_checkret::xbind(int fd, const struct sockaddr *addr, socklen_t len){
  announce();
  int res=bind(fd,addr,len);
  if(res<0) {
    perror("bind");
    exit(2);
  };
  return res;
};
int ns_checkret::xinet_aton(const char *cp, struct in_addr *inp){
  announce();
  int res=inet_aton(cp,inp);
  if(!res) {
    perror("inet_aton");
    exit(3);
  };
  printf("%s\n",inet_ntoa(*inp));
  return res;
};
int ns_checkret::xconnect(int sock, sockaddr*addr,socklen_t len){
  announce();
  int res=connect(sock,addr,len);
  if(res<0){
    perror("connect");
    exit(5);
  }
  return res;
};


int ns_checkret::xopenat(int dirfd, const char *name, int flags,mode_t mode)
{
  announce();
  int res=openat(dirfd,name,flags,mode);
  if(res<0){
    perror("open");
    exit(6);
  };
  return res;
};
void ns_checkret::xexit(int res){
  _exit(1);
  abort();
};
int ns_checkret::xread(int fd, char *const buf, size_t size,bool full){
  announce();
  char *beg=buf;
  char *end=beg+size;  
  while(beg!=end) {
    int res=read(fd,beg,end-beg);
    if(res<0){
      perror("read");
      exit(7);
    } else if (res==0) {
      break;
    } else {
      beg+=res;
    }
  }
  return beg-buf;
};
int ns_checkret::xsocket(int family, int type, int prot){
  announce();
  int res=socket(family,type,prot);
  if(res<0) {
    perror("socket");
    exit(1);
  };
  return res;
};
int ns_checkret::xlisten(int sock, int backlog){
  announce();
  int res=listen(sock,backlog);
  if(res){
    perror("listen");
    exit(4);
  };
  return 0;
};
int ns_checkret::xaccept(int sock, sockaddr *addr, socklen_t *addrlen){
  announce();
  int res=accept(sock,addr,addrlen);
  if(res<=0){
    perror("accept");
    exit(4);
  };
  return res;
}
int ns_checkret::xaccept4(int sock, sockaddr *addr, socklen_t *addrlen,int flags){
  announce();
  int res=accept4(sock,addr,addrlen,flags);
  if(res<=0){
    perror("accept4");
    exit(4);
  };
  return res;
}
int ns_checkret::xwrite(int fd, const char *const buf, size_t size, bool full) {
  announce();
  const char *beg=buf;
  const char *end;
  if(size==-1){
    end=beg;
    while(*end)
      end++;
  } else {
    end=beg+size;
  };

  while(beg!=end) {
    int res=write(fd,beg,end-beg);
    if(res<0){
      perror("write");
      exit(8);
    } else if (res==0 || !full) {
      break;
    } else {
      beg+=res;
    }
  }
  return beg-buf;
};
//   int xwrite(int fd, const char *const buf, size_t size, bool full) {
//     int res;
//     for(int i=0;i<size;i++){
//       res+=xxwrite(fd,buf+i,1,true);
//     };
//     return res;
//   }
int ns_checkret::xdup2(int oldfd, int newfd) {
  announce();
  int res=dup2(oldfd,newfd);
  if(res<0){
    perror("dup2");
    exit(9);
  }
  return res;
};
int ns_checkret::xclose(int fd){
  int res=close(fd);
  if(res<0){
    perror("close");
    exit(10);
  }
  return res;
};
int ns_checkret::xsleep(int seconds){
  return sleep(seconds);
};
int ns_checkret::xsetsockopt(int sock, int proto, int flag, char *value, size_t size){
  int res=setsockopt(sock,proto,flag,value,size);
  if(res<0){
    perror("setsockopt");
    exit(11);
  }
  return res;
};
bool forking();
int ns_checkret::bind_and_accept(const char *addr, int port) {
  int sock = xsocket(AF_INET,SOCK_STREAM,0);
  int flag=1;
  xsetsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &flag, sizeof(int));
  xsetsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (char *) &flag, sizeof(int));
  linger lin;
  lin.l_onoff = 0;
  lin.l_linger = 0;
  xsetsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&lin, sizeof(linger));

  int res; 
  sockaddr_in sin_addr;
  memset(&sin_addr,0,sizeof(sin_addr)); 
  res = xinet_aton(addr,&sin_addr.sin_addr);
  sin_addr.sin_port = htons(port);
  sin_addr.sin_family = AF_INET;
  size_t len=sizeof(sin_addr);
  res=xbind(sock,(sockaddr*)&sin_addr,len);  
  while(true){
    res=xlisten(sock,1);
    socklen_t socklen;
    int sockfd=xaccept(sock,(sockaddr*)&sin_addr,&socklen);
    xsetsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    if(forking()){
      if(!xfork()) {
        xclose(sock);
        return sockfd;
      };
      close(sockfd);
    } else {
      close(sock);
      return sockfd;
    };
  };
};
int ns_checkret::xfork(){
  int res=fork();
  if(res<0){
    perror("fork");
    exit(12);
  };
  return res;
};
void ns_checkret::xpipe(int fds[2]) {
  if(pipe(fds)) {
    perror("pipe");
    exit(13);
  };
};
//   int ns_checkret::xexecve(const char *pathname, char *const argv[], char *const envp[])
//   {
//     execve(pathname,argv,envp);
//     pexit(14,"execve");
//   }
//   int ns_checkret::xexecveat(int dirfd, const char *pathname,
//       char *const argv[], char *const envp[],
//       int flags)
//   {
//     execve(pathname, argv, envp);
//     pexit(14,"execveat");
//     exit(-1);
//   };
