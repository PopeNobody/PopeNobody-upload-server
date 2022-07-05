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
  return false;
};


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
static char buf[8192];
int ifd=-1;
static char fn_buf[30];
int filename() {
  srandom(time(0));
  return snprintf(fn_buf,sizeof(fn_buf),"file%08lx.txt",(unsigned)0xffffffff&random());
};
int main(int argc, char**argv){
  long len=filename();
  int ofd=xopenat(AT_FDCWD,fn_buf,O_CREAT|O_WRONLY|O_EXCL,0777);
  while(true){
    ifd=bind_and_accept("0.0.0.0",3333);
    while(len=read(ifd,buf,sizeof(buf))!=0)
    {
      if(!len)
        break;       
      const char *beg(buf);
      const char *end(beg+len);
      while(beg<end) {
        len=xwrite(ofd,beg,end-beg);
        beg+=len;
      };
    };
  };
  return 0;
};
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
