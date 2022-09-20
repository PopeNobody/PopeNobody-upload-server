#include "md5.hh"
#include "md5.h"
#include "util.hh"
#include <string.h>
#include "unixpp.hh"

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
