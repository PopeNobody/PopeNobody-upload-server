#include "unixpp.hh"
#include "checkret.hh"
#include <sys/mman.h>
#include <fcntl.h>

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
