require "tools/modules/qt"

solution "pdb-type-viewer"

    configurations {
        "debug",
        "release",
    }

    platforms {
        "x64",
    }
    
    defaultplatform "x64"

    -- common directories
    objdir "%{wks.location}/build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.platform}/obj"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.platform}/bin"
    debugdir "$(TargetDir)"

    systemversion "latest"
    warnings "extra"
    cppdialect "c++latest"

    flags {
        "MultiProcessorCompile",
        "FatalWarnings",
        "NoManifest",
    }

    linkoptions {
        "/ignore:4221", -- LNK4221: This object file does not define any previously undefined public symbols, so it will not be used by any link operation that consumes this library
        "/ignore:4006", -- LNK4006: __NULL_IMPORT_DESCRIPTOR already defined in dbghelp.lib(dbghelp.dll); second definition ignored
        "/ignore:4099", -- LNK4099: PDB '' was not found with 'qtmain.lib(qtmain_win.obj)' or at ''; linking object as if no debug info
    }
    
    buildoptions {
        "/permissive-",
        "/bigobj",
        "/Zc:inline",
        "/Zc:throwingNew",
        "/Zc:__cplusplus",  -- https://devblogs.microsoft.com/cppblog/msvc-now-correctly-reports-__cplusplus/
    }

    filter { "action:vs2019" }
        buildoptions {
            "/Zc:char8_t",      -- NOTE: This one must be removed once MSVC will support it on regular basis.
        }

    filter { "configurations:debug" }
        defines {
            "DEBUG",
            "_DEBUG",
        }
        runtime "debug"
        symbols "on"

    filter { "configurations:release" }
        defines {
            "NDEBUG",
        }
        runtime "release"
        optimize "speed"
        inlining "auto"

    filter { "kind:ConsoleApp or WindowedApp" }
        linkgroups "on"


filter {}

include 'source'
