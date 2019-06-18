project "libptv"
    kind "SharedLib"

    files {
        "public/**.hxx",
        "private/**.cxx",
        "*.lua",
    }

    includedirs {
        "%{wks.location}/source/lib/public",
        "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/DIA SDK/include",
    }

    libdirs {
        "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/DIA SDK/lib/amd64"
    }

    links {
        "diaguids.lib",
    }

    language "c++"
