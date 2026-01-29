#include "memory.h"

int memoryGetInfo(FILE* meminfo_proc_file, MemoryInfo* mem_info)
{
    if (!meminfo_proc_file || !mem_info)
    {
        return -1;
    }

    fscanf(meminfo_proc_file, "MemTotal:\t%lf kB\n", &mem_info->total);
    fscanf(meminfo_proc_file, "%*[^\n]\n");
    fscanf(meminfo_proc_file, "MemAvailable:\t%lf kB", &mem_info->free);

    mem_info->total /= 1024.0;
    mem_info->free /= 1024.0;
    mem_info->used = mem_info->total - mem_info->free;

    return 0;
}
