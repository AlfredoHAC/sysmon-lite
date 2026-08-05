#include "cpu.h"

#include <stdio.h>
#include <unistd.h>

enum
{
    MAX_LINE_LENGTH = 256
};

static long sum_total_cpu_usage(const CpuTimes cpu_time)
{
    return cpu_time.user + cpu_time.nice + cpu_time.system + cpu_time.idle + cpu_time.iowait + cpu_time.irq +
           cpu_time.softirq + cpu_time.steal;
}

static long sum_total_cpu_idle(const CpuTimes cpu_time)
{
    return cpu_time.idle + cpu_time.iowait;
}

static double calculate_cpu_usage_percentage(const CpuInfo* cpu_info)
{
    long total_cpu_time = sum_total_cpu_usage(cpu_info->current);
    long prev_total_cpu_time = sum_total_cpu_usage(cpu_info->prev);
    long total_cpu_time_delta = total_cpu_time - prev_total_cpu_time;

    long total_cpu_idle_time = sum_total_cpu_idle(cpu_info->current);
    long prev_total_cpu_idle_time = sum_total_cpu_idle(cpu_info->prev);
    long total_cpu_idle_time_delta = total_cpu_idle_time - prev_total_cpu_idle_time;

    if (total_cpu_time_delta == 0.0)
    {
        return 0.0;
    }

    return (double)((total_cpu_time_delta - total_cpu_idle_time_delta) * 100) / (double)total_cpu_time_delta;
}

int cpuGetInfo(FILE* cpu_proc_file, CpuInfo* cpu_info)
{
    if (!cpu_proc_file || !cpu_info)
    {
        return -1;
    }

    char line_buffer[MAX_LINE_LENGTH] = {0};

    cpu_info->prev = cpu_info->current;

    fgets(line_buffer, sizeof(line_buffer), cpu_proc_file);
    sscanf(line_buffer, "%*[^\x20] %ld %ld %ld %ld %ld %ld %ld %ld", &cpu_info->current.user, &cpu_info->current.nice,
           &cpu_info->current.system, &cpu_info->current.idle, &cpu_info->current.iowait, &cpu_info->current.irq,
           &cpu_info->current.softirq, &cpu_info->current.steal);

    cpu_info->percentage = calculate_cpu_usage_percentage(cpu_info);

    return 0;
}
