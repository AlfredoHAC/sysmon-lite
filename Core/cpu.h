#ifndef CPU_HEADER_H
#define CPU_HEADER_H

#include <stdio.h>

typedef struct CpuTimes
{
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
    long steal;
} CpuTimes;

typedef struct CpuInfo
{
    CpuTimes prev;
    CpuTimes current;
    double percentage;
} CpuInfo;

int cpuGetInfo(FILE* cpu_proc_file, CpuInfo* cpu_info);

#endif
