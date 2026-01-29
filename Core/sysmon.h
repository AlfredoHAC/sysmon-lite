#ifndef SYSMON_HEADER_H
#define SYSMON_HEADER_H

#include <stdbool.h>

bool sysmonInit();
void sysmonRun();
void sysmonStop();
void sysmonTerminate();

#endif // SYSMON_HEADER_H
