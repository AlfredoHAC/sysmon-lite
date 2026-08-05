#include "src/sysmon.h"

int main(void) {
  sysmonInit();

  sysmonRun();

  sysmonTerminate();
  return 0;
}
