#include <errno.h>
#include <fcntl.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "ether.h"
#include "util.h"

#define CLONE_DEVICE "/dev/net/tun"

#ifndef IFF_TAP
#define IFF_TAP 0x0002
#endif
#ifndef IFF_NO_PI
#define IFF_NO_PI 0x1000
#endif
#ifndef TUNSETIFF
#define TUNSETIFF 0x400454ca
#endif

int main(int argc, char *argv[]) {
  int fd;
  char *ifname;
  struct ifreq ifr;
  uint8_t buf[2048];
  ssize_t n;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <interface>\n", argv[0]);
    return 1;
  }

  fd = open(CLONE_DEVICE, O_RDWR);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  ifname = argv[1];
  strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name) - 1);
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  if (ioctl(fd, TUNSETIFF, &ifr) == -1) {
    errorf("ioctl [TUNSETIFF] %s", strerror(errno));
    close(fd);
    return 1;
  }
  infof("waiting for packets from <%s>...", ifname);
  while (1) {
    n = read(fd, buf, sizeof(buf));
    if (n == -1) {
      if (errno == EINTR) {
        continue;
      }
      errorf("recv: %s", strerror(errno));
      close(fd);
      return -1;
    }
    debugf("received %zu bytes", n);
    ether_print(buf, n);
  }
  close(fd);
  return 0;
}
