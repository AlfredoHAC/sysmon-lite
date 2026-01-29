#ifndef UPTIME_HEADER_H
#define UPTIME_HEADER_H

#include <stdio.h>

typedef struct UptimeInfo
{
    int days;
    int hours;
    int minutes;
    int seconds;
} UptimeInfo;

int uptimeGetInfo(FILE* uptime_proc_file, UptimeInfo* uptime_info);

#endif
