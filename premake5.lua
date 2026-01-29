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
        "**.c",
        "src/**.c"
    }

    includedirs {
        "**.h",
        "src/**.h"
    }

    symbols "On"

    objdir "bin/obj"
