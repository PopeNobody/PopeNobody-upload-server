#include "checkret.hh"
#include "fixed_buf.hh"
#include <fcntl.h>
#include "md5.h"

using namespace checkret;

extern "C" {
  int dprintf(int, const char *, ...);
  int atoi(const char *);
}
bool forking(){
  return false;
};

static char buf[8];
int ifd=-1;
static char fn_buf[30];
int filename() {
  srandom(time(0));
  return snprintf(fn_buf,sizeof(fn_buf),"upload/%08lx.bin",(long)getpid());
};
size_t total=0;
int main(int argc, char**argv){
  ifd=bind_and_accept("0.0.0.0",3333);
  while(true){
    filename();
    xmkdir("upload",0777);
    int ofd=xopenat(AT_FDCWD,fn_buf,O_CREAT|O_WRONLY|O_EXCL,0666);
    write_cs(1,"accepted\n");
    while(true){
      size_t rlen=read(ifd,buf,sizeof(buf));
      const char *beg(buf);
      const char *end=beg+rlen;
      total+=rlen;
      if(beg==end)
        break;
      while(beg<end)
        beg+=xwrite(ofd,beg,end-beg);
    };
    dprintf(1,"wrote %lu bytes\n",total);
    return 0;
  };
};
