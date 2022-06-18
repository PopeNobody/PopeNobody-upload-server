#include "checkret.hh"

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
int xconnect(int sock, sockaddr*addr,socklen_t len){
  int res = connect(sock,addr,len);
  if(res<0){
    perror("connect");
    exit(5);
  }
  return res;
};
int xopen(const char *name, int flags){
  int res=open(name,flags);
  if(res<0){
    perror("open");
    exit(6);
  };
  return res;
};
int xread(int fd, char *buf, size_t size){
  int res=read(fd,buf,size);
  if(res<0){
    perror("read");
    exit(7);
  }
  return res;
};
int xsocket(int family, int type, int prot){
  int res=socket(family,type,prot);
  if(res<0) {
    perror("socket");
    exit(1);
  };
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
