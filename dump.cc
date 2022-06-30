#include "checkret.hh"
#include "fixed_buf.hh"
#include <fcntl.h>
#include "md5.h"

using namespace ns_checkret;

extern "C" {
  int dprintf(int, const char *, ...);
  int atoi(const char *);
}
bool forking(){
  return true;
};


static char buf[8192];
int ifd=-1;
int xforkpipe(){
  int fds[2];
  xpipe(fds);
  if(xfork()){
    xdup2(fds[0],0);
    xclose(fds[0]);
    xclose(fds[1]);
    xclose(ifd);
    const char *const argv[2] = {
      "/usr/bin/tee",
      0
    };
    const char **envp = (const char**)environ;
    xexecve("/usr/bin/tee", (const char**)argv, (char * const *)envp);
    pexit(1,"exec");
  } else {
    xdup2(fds[1],1);
    xclose(fds[0]);
    xclose(fds[1]);
  }
  return 0;
};

int main(int argc, char**argv){
  ifd=bind_and_accept("0.0.0.0",3333);
  int ofd=1;
  fixed_buf<20> fmt_now;
  format_now(fmt_now);
  xforkpipe();
  xwrite(ifd,"Filename: ",-1,true);
  while(true){
    int len=read(ifd,buf,sizeof(buf));
    if(len==0)
      break;
    const char *beg(buf);
    const char *end(beg+len);
    while(beg<end) {
      len=write(ofd,beg,end-beg);
      if(!len)
        pexit(1,"0 read");
      beg+=len;
    };
  };
  return 0;
};
