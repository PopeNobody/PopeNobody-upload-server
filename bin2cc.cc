#include "unixpp.hh"
#include "checkret.hh"

using unixpp::range_t;
using unixpp::xmmap_file;

static char hex_digs[]="0123456789abcdef";

struct hex_t {
  char text[4];
  static unsigned char next;
  hex_t(unsigned char ch=next++)
  {
    text[0]=hex_digs[0xf&(ch>>4)];
    text[1]=hex_digs[0xf&ch]; 
    text[2]=0;
    text[3]=0;
  }
  operator const char*() const {
    return text;
  };
};
unsigned char hex_t::next=0;
hex_t hex_pairs[256];
using namespace checkret;

int main(int, char**){
  range_t bash=xmmap_file("/bin/bash-static");
  size_t size=bash.size();
  int fd = xopen("bash-static.cc",O_RDWR);
  xftruncate(fd,size*2);
  range_t bash_src=xmmap_file("bash-static.cc");
  close(fd);
  for(auto b(bash.beg()),e(bash.end());b!=e;b++){
    
  }  
  return 0;
}
