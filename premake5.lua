workspace "ToyEngine"
    architecture "x64"
    startproject "ToyEngine"
   
    configurations
    {
     "Debug",
     "Release",
    }

outputdir = "%{cfg.buildcfg}"

IncludeDir={}
IncludeDir["sfml"]="ToyEngine/vendor/sfml/include/"
IncludeDir["imgui"]="ToyEngine/vendor/imgui"

LibDir={}
LibDir["sfml"]="ToyEngine/vendor/sfml/lib"


project "ToyEngine"
   location "ToyEngine"
   kind "ConsoleApp"
   language "c++"
   staticruntime "off"
   cppdialect "c++17"
   buildoptions { "/utf-8" }

   targetdir ("%{prj.name}/bin/"..outputdir.."/%{prj.name}")
   objdir ("%{prj.name}/bin-int/"..outputdir.."/%{prj.name}")

   files
   {
    "%{prj.name}/src/A2/**.h",
    "%{prj.name}/src/A2/**.cpp",
    "%{prj.name}/src/main.cpp",
    "%{IncludeDir.imgui}/**.h",
    "%{IncludeDir.imgui}/**.cpp"
   }

   includedirs
   {
    "%{prj.name}/src",
    "%{IncludeDir.sfml}",
    "%{IncludeDir.imgui}",
   }

   libdirs
   {
    "%{LibDir.sfml}",
   }

   links
   {
    "opengl32.lib"
   }

   filter "system:windows"
        systemversion "latest"

   filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

        links
        {
            "sfml-graphics-d",
            "sfml-system-d",
            "sfml-window-d",
        }

        postbuildcommands
        {
            '{COPYFILE} "vendor/sfml/bin/sfml-graphics-d-3.dll" "%{cfg.targetdir}"',
            '{COPYFILE} "vendor/sfml/bin/sfml-system-d-3.dll" "%{cfg.targetdir}"',
            '{COPYFILE} "vendor/sfml/bin/sfml-window-d-3.dll" "%{cfg.targetdir}"',
        }

   filter "configurations:Release"
        runtime "Release"
        optimize "on"

        links
        {
            "sfml-graphics",
            "sfml-system",
            "sfml-window",
        }

        postbuildcommands
        {
            '{COPYFILE} "vendor/sfml/bin/sfml-graphics-3.dll" "%{cfg.targetdir}"',
            '{COPYFILE} "vendor/sfml/bin/sfml-system-3.dll" "%{cfg.targetdir}"',
            '{COPYFILE} "vendor/sfml/bin/sfml-window-3.dll" "%{cfg.targetdir}"',
        }