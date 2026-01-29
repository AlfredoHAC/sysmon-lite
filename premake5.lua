---@diagnostic disable: undefined-global
workspace "Sysmon-Lite"
    filename "Sysmon-Lite"
    configurations { "Final" }

project "Sysmon-Lite"
    kind "ConsoleApp"
    language "C"
    cdialect "C23"

    links { "ncurses" }

    files {
        "**.c"
    }

    includedirs {
        "**.h"
    }

    symbols "On"

    objdir "bin/obj"
