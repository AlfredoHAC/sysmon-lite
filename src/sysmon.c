#include "sysmon.h"

#include "cpu.h"
#include "memory.h"
#include "uptime.h"

#include <locale.h>
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static volatile sig_atomic_t running = false;
static int cpu_core_count = 0;

static void signalHandler(int signal)
{
    sysmonStop();
}

bool sysmonInit()
{
    setlocale(LC_ALL,
              ""); // Sem isso, ncurses pode ficar instável e cheia de UB
                   // (Undefined Behavior, i.e. comportamentos indefinidos)

    initscr();
    start_color();
    cbreak();
    noecho();
    noqiflush();

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLUE);

    keypad(stdscr, true);
    curs_set(0);
    timeout(400);

    signal(SIGINT, signalHandler);
    cpu_core_count = sysconf(_SC_NPROCESSORS_CONF);

    running = true;
    return true;
}

void sysmonRun()
{
    int key = 0;
    int width, height;
    const char* welcome_str = "Welcome to Sysmon-Lite! (Press Q to quit)\n";

    FILE* proc_file = NULL;

    UptimeInfo uptime_info;
    MemoryInfo mem_info;
    CpuInfo cpus_infos[cpu_core_count + 1] = {};

    getmaxyx(stdscr, height, width);

    while (running)
    {
        key = getch();
        if (key == 'q' || key == 'Q')
        {
            sysmonStop();
            continue;
        }

        erase();

        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(2, (width / 2) - (strlen(welcome_str) / 2), "%s", welcome_str);
        attroff(A_BOLD | COLOR_PAIR(1));

        // Uptime - /proc/uptime

        proc_file = fopen("/proc/uptime", "r");
        if (!proc_file)
        {
            sysmonStop();
            continue;
        }

        if (uptimeGetInfo(proc_file, &uptime_info) == -1)
        {
            fclose(proc_file);
            sysmonStop();
            continue;
        }

        fclose(proc_file);

        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(4, 4, "Uptime: ");
        attroff(A_BOLD | COLOR_PAIR(1));

        attron(A_BOLD);
        printw("%d days and %02dh %02dm %02ds.", uptime_info.days, uptime_info.hours, uptime_info.minutes,
               uptime_info.seconds);
        attroff(A_BOLD);

        // Meminfo - /proc/meminfo
        proc_file = fopen("/proc/meminfo", "r");
        if (!proc_file)
        {
            sysmonStop();
            continue;
        }

        if (memoryGetInfo(proc_file, &mem_info) == -1)
        {
            fclose(proc_file);
            sysmonStop();
            continue;
        }

        fclose(proc_file);

        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(6, 4, "Memory Usage:\n");
        attroff(A_BOLD | COLOR_PAIR(1));

        attron(A_BOLD | COLOR_PAIR(2));
        mvprintw(8, 5, "-> ");
        attroff(A_BOLD | COLOR_PAIR(2));

        printw("Total: ");
        attron(A_BOLD);
        printw("%5.1f GB (%8.1f MB)\n", mem_info.total / 1024, mem_info.total);
        attroff(A_BOLD);

        attron(A_BOLD | COLOR_PAIR(2));
        mvprintw(9, 5, "-> ");
        attroff(A_BOLD | COLOR_PAIR(2));

        printw("Free: ");
        attron(A_BOLD);
        printw("%6.1f GB (%8.1f MB)\n", mem_info.free / 1024, mem_info.free);
        attroff(A_BOLD);

        attron(A_BOLD | COLOR_PAIR(2));
        mvprintw(10, 5, "-> ");
        attroff(A_BOLD | COLOR_PAIR(2));

        printw("Used: ");
        attron(A_BOLD);
        printw("%6.1f GB (%8.1f MB, %3.2lf%% used)\n", mem_info.used / 1024, mem_info.used,
               mem_info.total == 0 ? 0.0 : (mem_info.used / mem_info.total) * 100.0);
        attroff(A_BOLD);

        // CPU Stats - /proc/stat
        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(12, 4, "CPU Usage%%:");
        attroff(A_BOLD | COLOR_PAIR(1));

        attron(A_BOLD | COLOR_PAIR(2));
        mvprintw(14, 5, "-> ");
        attroff(A_BOLD | COLOR_PAIR(2));

        proc_file = fopen("/proc/stat", "r");
        if (!proc_file)
        {
            sysmonStop();
            continue;
        }

        for (int i = 0; i <= cpu_core_count; i++)
        {
            if (i == 0)
            {
                if (cpuGetInfo(proc_file, &cpus_infos[i]) == -1)
                {
                    fclose(proc_file);
                    sysmonStop();
                    break;
                }

                printw("CPU Overall%%: ");
                attron(A_BOLD);
                printw("%5.2lf%%", cpus_infos[i].percentage);
                attroff(A_BOLD);

                attron(A_BOLD | COLOR_PAIR(1));
                mvprintw(16, 5, "Per CPU%%:");
                attroff(A_BOLD | COLOR_PAIR(1));

                continue;
            }

            attron(A_BOLD | COLOR_PAIR(2));
            mvprintw(16 + i, 6, "-> ");
            attroff(A_BOLD | COLOR_PAIR(2));

            if (cpuGetInfo(proc_file, &cpus_infos[i]) == -1)
            {
                fclose(proc_file);
                sysmonStop();
                break;
            }

            printw("CPU %2d: ", i - 1);
            attron(A_BOLD);
            printw("%5.2lf%%", cpus_infos[i].percentage);
            attroff(A_BOLD);
        }

        fclose(proc_file);

        attron(COLOR_PAIR(1));
        box(stdscr, 0, 0);
        attroff(COLOR_PAIR(1));

        refresh();
    }
}

void sysmonStop()
{
    running = false;
    sysmonTerminate();
}

void sysmonTerminate()
{
    endwin();
}
