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
IncludeDir["vendor"] = "%{wks.location}/Axis/vendor"
IncludeDir["spdlog"] = "%{wks.location}/Axis/vendor/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Axis/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Axis/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Axis/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Axis/vendor/glm"

group "Dependencies"
  include "Axis/vendor/GLFW"
  include "Axis/vendor/Glad"
  include "Axis/vendor/imgui"
group ""

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
    "%{IncludeDir.vendor}",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.glm}"
  }
  
  links
  {
    "GLFW",
    "Glad",
    "ImGui"
  }
  
  defines
  {
    "GLFW_INCLUDE_NONE",
    "_CRT_SECURE_NO_WARNINGS"
  }
 
  filter "system:linux"
    pic "on"
    systemversion "latest"
  
    links
    {
      "Xrandr",
      "Xi",
      "GL",
      "X11",
      "OpenGL"
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
    symbols "on"

  filter "configurations:Release"
    defines "AXIS_RELEASE"
    runtime "Release"
    optimize "on"

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
    "%{IncludeDir.vendor}",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.glm}"
  }
  
  links
  {
    "Axis",
    "ImGui"
  }
    

  filter "system:linux"
    systemversion "latest"
    
    links
    {
      "dl",
      "pthread",
      "OpenGL",
      "Glad",
      "GLFW"
    }
    
  filter "system:windows"
    systemversion "latest"

  filter "configurations:Debug"
    defines "AXIS_DEBUG"
    runtime "Debug"
    symbols "on"

  filter "configurations:Release"
    defines "AXIS_RELEASE"
    runtime "Release"
    optimize "on"

  filter "configurations:Dist"
    defines "AXIS_DIST"
    runtime "Release"
    optimize "Full"
