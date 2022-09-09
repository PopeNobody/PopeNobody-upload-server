//#include "stderr.h"
#include <sys/un.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "fd-path.h"
#include "checkret.hh"

using checkret::xopen;
using checkret::xlseek;
using checkret::xclose;
using checkret::xbind;
using checkret::xlisten;
using checkret::xaccept;


void err_remark(const char *msg){
  dprintf(2,"%s\n",msg);
};
void err_syserr(const char *msg){
  dprintf(2,"%s\n",msg);
};
static
void wyslij(int socket, int fd)  // send fd by socket
{
    struct msghdr msg = { 0 };
    char buf[CMSG_SPACE(sizeof(fd))];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = { .iov_base = (void*)"ABC", .iov_len = 3 };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd));

    *((int *) CMSG_DATA(cmsg)) = fd;

    msg.msg_controllen = CMSG_SPACE(sizeof(fd));

    if (sendmsg(socket, &msg, 0) < 0)
        err_syserr("Failed to send message\n");
}

static
void wait_msg(int socket) {
  struct msghdr msg = {0};

  char m_buffer[256];
  struct iovec io = { .iov_base = m_buffer, .iov_len = sizeof(m_buffer) };
  msg.msg_iov = &io;
  msg.msg_iovlen = 1;

  char c_buffer[256];
  msg.msg_control = c_buffer;
  msg.msg_controllen = sizeof(c_buffer);

  if (recvmsg(socket, &msg, 0) < 0)
    err_syserr("Failed to receive message\n");
};
bool forking() {
  return false;
};
int main(int argc, char **argv)
{
  int s = socket(AF_UNIX, SOCK_STREAM, 0);
  address_t sun = unix_path();

  unlink(sun.sun.sun_path); 
  xbind(s,sun,sizeof(sun));

  xlisten(s,1);

  socklen_t size=sizeof(sun);
  while(true){
    const char *filename = "./checkret.hh";

    if (argc > 1)
      filename = argv[1];

    int fd = xopen(filename, O_RDONLY);

    int sock=xaccept(s,sun,&size);

    dprintf(2,"got connection\n");

    wyslij(sock, fd);
    xclose(fd);
  }
  struct timespec spec = { .tv_sec = 1, .tv_nsec = 500000000};
  nanosleep(&spec , 0);
  err_remark("Parent exits\n");
  return 0;
}
