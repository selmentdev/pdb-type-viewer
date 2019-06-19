project "libptv"
    kind "SharedLib"

    files {
        "public/**.hxx",
        "private/**.cxx",
        "*.lua",
    }

    includedirs {
        "%{wks.location}/source/lib/public",
        "$(VSInstallRoot)/DIA SDK/include",
    }

    libdirs {
        "$(VSInstallRoot)/DIA SDK/lib/amd64"
    }

    links {
        "diaguids.lib",
    }

    language "c++"
