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
IncludeDir["spdlog"] = "%{wks.location}/Axis/vendor/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Axis/vendor/GLFW/include"

include "Axis/vendor/GLFW"

project "Axis"
  location "Axis"
  kind "StaticLib"
  language "C++"
  cppdialect "C++17"
  staticruntime "On"
 
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
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.GLFW}"
  }
  
  links
  {
    "GLFW"
  }
 
  filter "system:linux"
    pic "On"
    cppdialect "C++17"
    systemversion "latest"
  
    links
    {
      "Xrandr",
      "Xi",
      "GLEW",
      "GL",
      "X11",
      "OpenGL"
    }
 
  filter "system:window"
    systemversion "latest"
    
    links
    {
      "opengl32.lib"
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

project "SandBox"
  location "SandBox"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++17"
  staticruntime "On"

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
  
  links
  {
    "Axis"
  }
    

  filter "system:linux"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
    
    links
    {
      "dl",
      "pthread",
      "OpenGL",
      "GLFW"
    }
    
  filter "system:windows"
    systemversion "latest"
    
    links
    {
      "opengl32.lib"
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
