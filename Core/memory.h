#ifndef MEMORY_HEADER_H
#define MEMORY_HEADER_H

#include <stdio.h>

typedef struct MemoryInfo
{
    double total;
    double free;
    double used;
} MemoryInfo;

int memoryGetInfo(FILE* meminfo_proc_file, MemoryInfo* mem_info);

#endif
