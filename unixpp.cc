#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include "checkret.hh"
#include "md5.hh"
#include "unixpp.hh"
#include "util.hh"

using unixpp::range_t;
using checkret::xopenat;
using checkret::xmmap;


range_t unixpp::xmmap_file(const char*fname) {
  range_t res;
  int fd=xopenat(AT_FDCWD,fname,O_RDONLY);
  size_t size=lseek(fd,0,SEEK_END);
  res.beg=(const char*)xmmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
  res.end=res.beg+size;
  return res;
};

using unixpp::range_t;
using unixpp::xmmap_file;
using unixpp::md5text;
using std::string;

string unixpp::md5sum(const unixpp::range_t &range)
{
  unsigned char resbuf[16];
  md5_ctx ctx;
  md5_init_ctx(&ctx);
  md5_process_bytes( range.beg,range.end-range.beg,&ctx);
  md5_finish_ctx(&ctx,&resbuf);
  char buffer[34];
  for(int i=0;i<16;i++){
    unsigned val = resbuf[i]&0xff;
    sprintf(buffer+2*i,"%02x",val);
  };
  return string(buffer);
}
string unixpp::md5file(const char *file) {
  range_t range=xmmap_file(file);
  return md5sum(range);
};
