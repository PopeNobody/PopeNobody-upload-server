#include <algorithm>
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

char this_shit[]="\x01\x02\x03\x04\x05\x06";

int main(int, char**){
  //range_t bash=xmmap_file("/bin/bash-static");
  //range_t bash=range_t(hex_digs,16);
  range_t bash=range_t(this_shit);
  dprintf(2,"%ld\n",bash.size());
//  range_t bash=range_t(hex_digs,16);
  size_t size=bash.size();
  int fd = xopen("bash-static.xx",O_CREAT|O_RDWR);
  xftruncate(fd,size*2);
  range_t bash_src=xmmap_file("bash-static.xx",true);
  close(fd);
  auto d((char*)bash_src.beg());
  for(auto b(bash.beg()),e(bash.end());b!=e;b++){
    const hex_t &hex=hex_pairs[*b];
    dprintf(2,"%s\n",bash_src.beg());
    *d++=hex[0];
    *d++=hex[1];
  }  
  return 0;
}
