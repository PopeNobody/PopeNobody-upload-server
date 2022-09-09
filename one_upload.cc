#include "checkret.hh"
#include "fixed_buf.hh"
#include <fcntl.h>
#include "md5.hh"
#include <sys/signal.h>
#include <wait.h>

using namespace checkret;

extern "C" {
  int dprintf(int, const char *, ...);
  int atoi(const char *);
}
bool forking(){
  return true;
};

static char buf[8];
int ifd=-1;
template<size_t size>
int filename(fixed_buf<size> &fn_buf) {
  return snprintf(fn_buf.buf,sizeof(fn_buf),"upload/%s-%08d.bin",now(),getpid());
};
size_t total=0;
int wait_nohang(int *stat) {
  int res = waitpid(-1,stat,WNOHANG);
  return res;
}
void sigchild(int arg){
  dprintf(1,"sigchild(%d)\n",arg);
  int wstat;
  dprintf(1,"wait_nohang(&wstat)=>%d\n", wait_nohang(&wstat));
  dprintf(1,"             wstat =>%d\n", wstat);
};
int main(int argc, char**argv){
  int port=0;
  if(argc!=2) {
    dprintf(2,"usage: %s <port>\n",argv[0]);
    exit(1);
  }
  port=atoi(argv[1]);
  signal(SIGCHLD, &sigchild);
  ifd=bind_and_accept("0.0.0.0",port);
  while(true){
    fixed_buf<60> fn_buf;
    filename(fn_buf);
    xmkdir("upload",0777);
    int ofd=xopenat(AT_FDCWD,fn_buf.buf,O_CREAT|O_WRONLY|O_EXCL,0666);
    while(true){
      size_t rlen=xread(ifd,buf,sizeof(buf));
      const char *beg(buf);
      const char *end=beg+rlen;
      total+=rlen;
      if(beg==end)
        break;
      while(beg<end)
        beg+=xwrite(ofd,beg,end-beg);
    };
    dprintf(1,"%s: wrote %lu bytes to %s\n",now(),total,fn_buf.buf);
    xclose(1);
    dprintf(ifd,"%s: wrote %lu bytes to %s\n",now(),total,fn_buf.buf);
    xclose(ifd);
    return 0;
  };
};
