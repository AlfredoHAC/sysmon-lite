#include "uptime.h"

enum
{
    SECONDS_PER_MINUTE = 60,
    SECONDS_PER_HOUR = 3600,
    SECONDS_PER_DAY = 86400,
    HOURS_PER_DAY = 24
};

int uptimeGetInfo(FILE* uptime_proc_file, UptimeInfo* uptime_info)
{
    if (!uptime_proc_file || !uptime_info)
    {
        return -1;
    }

    fscanf(uptime_proc_file, "%d", &uptime_info->seconds);

    uptime_info->minutes = (uptime_info->seconds % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
    uptime_info->hours = (uptime_info->seconds / SECONDS_PER_HOUR) % HOURS_PER_DAY;
    uptime_info->days = uptime_info->seconds / SECONDS_PER_DAY;
    uptime_info->seconds %= SECONDS_PER_MINUTE;

    return 0;
}
