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

    files
    {
        "src/**.cpp",
        "src/**.h",
        "lib/imgui/*.cpp",
        "lib/imgui/*.h"
    }

postbuildcommands
{
    "{COPYFILE} %{wks.location}/../lib/SDL/Windows/lib/*.dll %{cfg.targetdir}",
    "{MKDIR} %{cfg.targetdir}/assets",
    "{COPYDIR} %{wks.location}/../assets %{cfg.targetdir}/assets"
}

filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

 filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"