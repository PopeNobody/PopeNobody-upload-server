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


static char filename[128];
template<typename ch_t>
bool between(ch_t min, ch_t val, ch_t max){
  return !(val<min || val>max);
};
static bool safe_char(char ch){
  if(between('a',ch,'z'))
    return true;
  if(between('A',ch,'Z'))
    return true;
  if(between('0',ch,'0'))
    return true;
  switch(ch){
    case '-':
    case '_':
      return true;
    default:
      return false;
  };
};
//   void readFileName() {
//     char buf;
//     memset(filename,0,sizeof(filename));
//     int pos=0;
//     while(xread(ofd,&buf,1)==1){
//       if(buf==10) {
//         return;
//       } else if(safe_char(buf)) {
//         filename[pos++]=buf;
//       } else {
//         dprintf(ofd,"bad char: %d\n",buf);
//         xclose(ofd);
//         xexit(9);
//       };
//         
//     };
//   }
static char buf[8192];
int ifd=-1;
//   int xforkpipe(){
//     int fds[2];
//     xpipe(fds);
//     if(xfork()){
//       xdup2(fds[0],0);
//       xclose(fds[0]);
//       xclose(fds[1]);
//       xclose(ifd);
//       execl("/usr/bin/tee", "/usrbin/tee", filename, (char*)0);
//       pexit(1,"exec");
//     } else {
//       xdup2(fds[1],1);
//       xclose(fds[0]);
//       xclose(fds[1]);
//     }
//     return 0;
//   };

int main(int argc, char**argv){
  ifd=bind_and_accept("0.0.0.0",3333);
  snprintf(filename,sizeof(filename)-1,"adump.%d",getpid());
  int ofd=xopenat(AT_FDCWD,filename,O_CREAT|O_WRONLY|O_EXCL,0777);

  while(true){
    int len=read(ifd,buf,sizeof(buf));
    const char *beg(buf);
    const char *end(beg+len);
    while(beg<end) {
      len=xwrite(ofd,beg,end-beg);
      beg+=len;
    };
  };
  return 0;
};
