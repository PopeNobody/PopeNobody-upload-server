#ifndef unixpp_hh
#define unixpp_hh unixpp_hh

#include <array>
#include <unixpp.hh>
#include <string>

namespace unixpp {

  using std::string;

  struct range_t {
    char *_beg;
    char *_end;
    range_t(char *beg=0, char *end=0)
      :_beg(beg), _end(end?end:beg)
    {
    }
    const char *beg() const{
      return _beg;
    };
    const char *end() const{
      return _end;
    };
    size_t size() const {
      return end()-beg();
    }
    const char &operator[](int idx) const{
      return beg()[idx];
    }
  };
  struct range_vt {
  };

  range_t xmmap_file(const char*fname);


  using std::array;
  using unixpp::range_t;

  typedef array<char,32> md5text;

  string md5sum(const range_t &range);
  string md5file(const char *file);

  string magic_mime(const range_t &range);
  string magic_ext(const range_t &range);
}

#endif
