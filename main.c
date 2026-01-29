#include "Core/sysmon.h"
#include <ncurses.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
  sysmonInit();

  sysmonRun();

  sysmonTerminate();
  return 0;
}
