workspace "Simple2DGameEngine"
    architecture "x64"
    configurations {"Debug", "Release"}
    location "generated"

project "Simple2DGameEngine"
    location "generated"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("build")
    debugdir ("build")

    files
    {
        "src/**.cpp",
        "src/**.h",
        "lib/imgui/*.cpp",
        "lib/imgui/*.h"
    }

filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"

filter "system:macosx"
    externalincludedirs
    {
        "lib/SDL/Mac/SDL2.framework/Headers",
        "lib/SDL/Mac/SDL2_image.framework/Headers",
        "lib/SDL/Mac/SDL2_ttf.framework/Headers",
        "lib/lua/include",
        "lib/sol",
        "lib/glm",
        "lib/imgui"
    }

    frameworkdirs
    {
        "lib/SDL/Mac"
    }

    links
    {
        "lib/SDL/Mac/SDL2.framework",
        "lib/SDL/Mac/SDL2_image.framework",
        "lib/SDL/Mac/SDL2_ttf.framework"
    }

    embedAndSign 
    {
        "SDL2.framework",
        "SDL2_image.framework",
        "SDL2_ttf.framework"
    }

    xcodebuildsettings
    {
        ["OTHER_LDFLAGS"] = "../lib/lua/MacLib/liblua.a"
    }

    postbuildcommands
    {
        "{COPYDIR} %{wks.location}/../assets %{cfg.targetdir}"
    }

filter "system:windows"
    includedirs
    {
        "lib/SDL/Windows/include",
        "lib/lua/include",
        "lib/sol",
        "lib/glm",
        "lib/imgui"
    }
    libdirs
    {
        "lib/SDL/Windows/lib",
        "lib/lua"
    }

    links
    {
        "SDL2main",
        "SDL2",
        "SDL2_image",
        "SDL2_ttf",
        "lua54"
    }

    postbuildcommands
    {
        "{COPYFILE} %{wks.location}/../lib/SDL/Windows/lib/*.dll %{cfg.targetdir}",
        "{MKDIR} %{cfg.targetdir}/assets",
        "{COPYDIR} %{wks.location}/../assets %{cfg.targetdir}/assets"
    }