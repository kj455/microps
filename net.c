#include "platform.h"

#include "util.h"

int net_init(void) {
  infof("initialize...");
  if (platform_init() == -1) {
    errorf("platform_init() failure");
    return -1;
  }
  infof("success");
  return 0;
}

int net_run(void) {
  infof("startup...");
  if (platform_run() == -1) {
    errorf("platform_run() failure");
    return -1;
  }
  infof("success");
  return 0;
}

int net_shutdown(void) {
  infof("shutdown...");
  if (platform_shutdown() == -1) {
    errorf("platform_shutdown() failure");
    return -1;
  }
  infof("success");
  return 0;
}
