#include "checkret.hh"
#include "util.hh"
#include "fixed_buf.hh"
#include <fcntl.h>
#include "md5.h"
#include "md5.hh"
#include <sys/signal.h>
#include <wait.h>
#include "unixpp.hh"

using namespace checkret;
using unixpp::range_t;
using unixpp::xmmap_file;
using std::string;
extern "C" {
  int dprintf(int, const char *, ...);
  int atoi(const char *);
}

static char buf[8*1024];
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
  if(argc!=3) {
    dprintf(2,"usage: %s <oid> <port>\n",argv[0]);
    exit(1);
  }

  const string oid=argv[1];
  port=atoi(argv[2]);
  signal(SIGCHLD, &sigchild);
  ifd=bind_accept_nofork("0.0.0.0",port);
  fixed_buf<60> fn_buf;
  filename(fn_buf);
  mkdirat(AT_FDCWD,"upload",0777);
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

  range_t file=xmmap_file(fn_buf.buf);
  string md5sum=unixpp::md5sum(file);
  string ext = unixpp::magic_ext(file);
  for(auto b(ext.begin()),e(ext.end()); b!=e;b++){
    if(*b=='/'){
      ext=string(ext.begin(),b);
      break;
    }
  }
  string filename="avatar."+oid+"."+md5sum+"."+ext;

  string output="{ \"filename\": \"";
  output += filename;
  output += "\",\"md5\": \"";
  output += md5sum;
  output += "\",\"mime\": \"";
  output += unixpp::magic_mime(file);
  output += "\",\"ext\": \"";
  output += ext;
  output += "\"}";
  dprintf(ifd,"%s\n",output.c_str());
  xclose(ifd);
  dprintf(1,"%s\n",output.c_str());
  filename="upload/"+filename;
  unlink(filename.c_str());
  xlink(fn_buf.buf,filename.c_str());
  xunlink(fn_buf.buf);
  return 0;
}
