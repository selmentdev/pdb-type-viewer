local qt = premake.extensions.qt

project "ptv-ui"
    kind "WindowedApp"

    files {
        "public/**.hxx",
        "private/**.cxx",
        "*.lua",
    }

    language "c++"

    qt.enable()

    qtpath "D:/Qt/5.13.0/msvc2017_64"

    qtmodules {
        "core",
        "gui",
        "widgets",
    }

    links {
        "qtmain"
    }

    qtprefix "Qt5"

    debugenvs {
        "PATH=D:/Qt/5.13.0/msvc2017_64/bin;D:/Qt/5.13.0/msvc2017_64/plugins/platforms;%PATH%"
    }

    configuration { "debug" }
        qtsuffix "d"