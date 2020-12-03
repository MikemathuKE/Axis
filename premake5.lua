require "premake-codeblocks/codeblocks"

workspace "Axis"
 architecture "x64"
 startproject "SandBox"

 configurations
 {
  "Debug",
  "Release",
  "Dist"
 }

 flags
 {
  "MultiProcessorCompile"
 }
  
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include Directories
IncludeDir = {}
IncludeDir["spdlog"] = "Axis/vendor/spdlog/include"

project "Axis"
 location "Axis"
 kind "StaticLib"
 language "C++"
 cppdialect "C++17"
 staticruntime "on"
 
 targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
 objdir ("%{prj.location}/bin-int/" .. outputdir .. "/%{prj.name}")
 
 pchheader "axispch.h"
 pchsource "Axis/src/axispch.cpp"
 
 files
 {
   "%{prj.name}/src/**.h",
   "%{prj.name}/src/**.cpp"
 }
 
 includedirs
 {
   "%{prj.name}/src",
   "%{IncludeDir.spdlog}"
 }
 
 filter "system:linux"
  pic "On"
  cppdialect "C++17"
  staticruntime "On"
  systemversion "latest"
  
  links
  {
   "Xrandr",
   "Xi",
   "GLEW",
   "GL",
   "X11"
  }
 
 filter "system:window"
  systemversion "latest"
  
 filter "configurations:Debug"
  defines "AXIS_DEBUG"
  runtime "Debug"
  symbols "On"

 filter "configurations:Release"
  defines "AXIS_RELEASE"
  runtime "Release"
  optimize "On"

 filter "configurations:Dist"
  defines "AXIS_DIST"
  runtime "Release"
  optimize "Full"

project "SandBox"
  location "SandBox"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"

  targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{prj.location}/bin-int/" .. outputdir .. "/%{prj.name}")

  files
  {
    "%{prj.name}/src/**.h",
   "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "%{wks.location}/Axis/src",
    "%{IncludeDir.spdlog}"
  }

  
  filter "system:linux"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
    
    links
    {
      "Axis:static"
    }
    

  filter "system:windows"
    systemversion "latest"
    
    links
    {
      "Axis"
    }
    

  filter "configurations:Debug"
    defines "AXIS_DEBUG"
    runtime "Debug"
    symbols "On"

  filter "configurations:Release"
    defines "AXIS_RELEASE"
    runtime "Release"
    optimize "On"

  filter "configurations:Dist"
    defines "AXIS_DIST"
    runtime "Release"
    optimize "Full"
