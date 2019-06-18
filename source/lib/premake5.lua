project "libptv"
    kind "SharedLib"

    files {
        "public/**.hxx",
        "private/**.cxx",
        "*.lua",
    }

    language "c++"
