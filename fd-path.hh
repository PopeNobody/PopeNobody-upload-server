#ifndef fd_path_h
#define fd_path_h fd_path_h

#include <sys/un.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "checkret.hh"

union address_t {
  sockaddr_un sun;
  sockaddr    sad;
  operator sockaddr*() {
    return &sad;
  };
};

extern address_t unix_path();


#endif
