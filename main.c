#include "src/sysmon.h"

int main(int argc, char *argv[]) {
  sysmonInit();

  sysmonRun();

  sysmonTerminate();
  return 0;
}
