project "ptv"
    kind "ConsoleApp"

    files {
        "public/**.hxx",
        "private/**.cxx",
        "*.lua",
    }

    language "c++"

    links {
        "libptv",
    }

    includedirs {
        "%{wks.location}/source/lib/public",
        "./public",
    }
