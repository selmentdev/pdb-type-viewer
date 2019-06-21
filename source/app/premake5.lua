local qt = premake.extensions.qt

local qt_path = "d:/Qt/5.13.0/msvc2017_64"

project "ptv-ui"
    kind "WindowedApp"

    files {
        "public/**.hxx",
        "private/**.cxx",
        "*.lua",
    }

    language "c++"

    qt.enable()

    qtpath(qt_path)

    qtmodules {
        "core",
        "gui",
        "widgets",
    }

    links {
        "libptv",
    }

    includedirs {
        "%{wks.location}/source/lib/public",
        "./public",
    }

    qtprefix "Qt5"

    debugenvs {
        "PATH=" .. qt_path .. "/bin;" .. qt_path .. "/plugins/platforms;$(VSInstallRoot)/DIA SDK/bin/amd64;%PATH%"
    }

    filter { "configurations:debug" }
        qtsuffix "d"

        links {
            "qtmaind",
        }

    filter { "configurations:not debug" }
        links {
            "qtmain",
        }