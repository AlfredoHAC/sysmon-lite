cmake_minimum_required(VERSION 3.14)
project(Sysmon-Lite C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Sysmon-Lite
    main.c
    src/cpu.c
    src/memory.c
    src/sysmon.c
    src/uptime.c
)

target_include_directories(Sysmon-Lite PRIVATE src)
target_link_libraries(Sysmon-Lite PRIVATE ncurses)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Sysmon-Lite PRIVATE -g -Wall -Wextra -Wpedantic)
endif()
