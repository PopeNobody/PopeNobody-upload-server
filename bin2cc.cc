#include "unixpp.hh"
#include "checkret.hh"

using unixpp::range_t;
using unixpp::xmmap_file;

struct hex_t {
  char text[4];
  static unsigned char next;
  hex_t(unsigned char ch=next++)
  {
    text[0]=0xf&(ch>>4);
    text[1]=0xf&ch; 
    text[2]=0;
    text[3]=0;
  }
};
unsigned char hex_t::next=0;
hex_t hex_pairs[256];

int main(int, char**){
  range_t bash=xmmap_file("/bin/bash-static");
  size_t size=bash.size();
  for(auto b(bash.beg),e(bash.end);b!=e;b++){

  }  
  return 0;
}
