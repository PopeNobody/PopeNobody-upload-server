#include "checkret.hh"
#include "fixed_buf.hh"
#include <fcntl.h>
#include "md5.h"
#include <sys/signal.h>
#include <wait.h>

using namespace checkret;

extern "C" {
  int dprintf(int, const char *, ...);
  int atoi(const char *);
}
bool forking(){
  return false;
};

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
#include <magic.h>
void show_magic(const char *fname) {
  int argc=10;
  char **argv=(char**)&fname;
  magic_t cookie=magic_open(MAGIC_ERROR);
  magic_load(cookie,"/usr/lib/file/magic.mgc");
  dprintf(2,"%d: %s\n",magic_errno(cookie), magic_error(cookie));
  if(magic_errno(cookie)){
    dprintf(2,"%d: %s\n",magic_errno(cookie),magic_error(cookie));
    exit(1);
  };
  int null=xopenat(AT_FDCWD,"/dev/null",O_RDONLY);
  dup2(null,0);
  for(int i=1;i<argc;i++){
    int fd=xopenat(AT_FDCWD,argv[i],O_RDONLY);
    dup2(fd,0);
    close(fd);
    const char *desc=magic_file(cookie,0);
    dprintf(1,"%s\n",argv[i]);
    dprintf(1,"%s\n",desc);
    dup2(null,0);
  };
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
    dprintf(2,"%s: wrote %lu bytes to %s\n",now(),total,fn_buf.buf);
    xclose(2);
    dprintf(ifd,"%s: wrote %lu bytes to %s\n",now(),total,fn_buf.buf);
    xclose(ifd);
  //  magic_t cookie=magic_open(MAGIC_ERROR);
   // magic_list(cookie,"/usr/lib/file/magic.mgc");
    magic_t mime=magic_open(MAGIC_ERROR|MAGIC_MIME);
    magic_load(mime,0);

    dprintf(1,"file: %s\n", fn_buf.buf);
    char *text;
    text=strdup(magic_file(mime,fn_buf.buf));
    char *pos=text;
    while(1) {
      if(*pos==';')
        *pos=0;
      if(!*pos)
        break;
      ++pos;
    };
      
    dprintf(1,"mime: %s\n", text);
    itr_pair_t pair=xmmap_file(fn_buf.buf);
    char resbuf[32];
    md5_ctx ctx;
    md5_init_ctx(&ctx);
    dprintf(1,"size: %ld\n", pair.end-pair.beg);
    md5_process_bytes( pair.beg,pair.end-pair.beg,&ctx);
    md5_finish_ctx(&ctx,&resbuf);
    dprintf(1,"md5s: ");
    for(int i=0;i<16;i++){
      dprintf(1,"%02x",resbuf[i]&0xff);
    };
    dprintf(1,"\n");
    return 0;
  };
};
