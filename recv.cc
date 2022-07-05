#include "checkret.hh"
#include <fcntl.h>
#include "md5.h"

using namespace checkret;

//   char fname[128];
//   int xread_ln(int con) {
//     memset(fname,0,sizeof(fname));
//     while(true){
//       int pos=0;
//       int res=xread(con,fname+pos,1);
//       if(res!=1 || fname[pos]=='\n')
//         return true;
//     };
//   };
int readdel(int fd, char *buf, size_t size, char del){
  for(int i=0;i<size;i++){
    int res=xread(fd,buf+i,1);
    if(res==0)
      return 0;
    if(buf[i]!=del)
      continue;
    buf[i]=0;
    return i;
  };
  return 0;
};
extern "C" {
  int dprintf(int, const char *, ...);
  int atoi(const char *);
}
bool forking(){
  return false;
};
int open_out(int sockfd){
  char fname[128];
  memset(fname,0,sizeof(fname));
  snprintf(fname,12,"%08d",getpid());
  dprintf(2,"'%s'\n",fname);
  int res = readdel(sockfd,fname+8,sizeof(fname)-8,'\n');
  dprintf(2,"'%s'\n",fname);
  if(res==0)
    return -1;
  return xopenat(AT_FDCWD,fname,O_WRONLY|O_CREAT|O_EXCL,0755);
};
size_t read_bytes(int sockfd){
  char buf[128];
  int res = readdel(sockfd,buf,sizeof(buf),'\n');
  if(res==0) {
    exit(2);
  };
  size_t bytes = atol(buf);
  dprintf(2,"'%s' %lu\n",buf,bytes);
  return bytes;
};
char sendsum[1024];
int main(int argc, char**argv){
  int sockfd=bind_and_accept("0.0.0.0",3333);
  int res=xwrite(sockfd,"\n",1);
  int ofd=open_out(sockfd);
  size_t bytes=read_bytes(sockfd);
  dprintf(1,"%lu\n",bytes);
  res=xwrite(sockfd,"\n",1);
  res=readdel(sockfd,sendsum,sizeof(sendsum),'\n');
  dprintf(1,"%d\n",res);
  dprintf(1,"%s\n",sendsum);
  write(sockfd,"\n",1);
  int len=0;
  char buffer[100];
  char resblock[16];
  char md5sum[33];
  struct md5_ctx ctx;
  /* Initialize the computation context.  */
  md5_init_ctx (&ctx);
  while(len<bytes){
    int num = 1;sizeof(buffer);//random()%sizeof(buffer);
    res=xread(sockfd,buffer,num);
    md5_process_bytes (buffer, res, &ctx);
    if(!res) {
      xwrite(2,"Unexpected end of file",-1);
      return 1;
    };
    len+=res;
    res=xwrite(ofd,buffer,res,true);
  }
  /* Initialize the computation context.  */
  for(int i=0;i<sizeof(resblock);i++)
    sprintf(md5sum+2*i,"%02x",resblock[i]&0xff);
  dprintf(1,"('%s')\n",md5sum);
  write(1,"\n",1);
  dprintf(1,"res: %d\n",strcmp(md5sum,sendsum));
  return 0;
};
